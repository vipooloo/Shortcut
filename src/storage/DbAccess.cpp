#include "DbAccess.h"
#include "ShortcutLogger.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <cinttypes>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sqlite3.h>

namespace shortcut {

bool DbAccess::StrToInt32(const char* str, int32_t& out_val)
{
    out_val = 0;
    if (!str || *str == '\0')
    {
        return false;
    }

    char* end = nullptr;
    const long val = std::strtol(str, &end, 10);
    if (end == str || *end != '\0')
    {
        return false;
    }

    if (val < INT32_MIN || val > INT32_MAX)
    {
        return false;
    }

    out_val = static_cast<int32_t>(val);
    return true;
}

DbResult DbAccess::Init()
{
    DbResult result{};
    bool is_ok{true};

    if (is_ok && m_db_path.empty())
    {
        result.SetCode(DbCode::INVALID_ARG);
        is_ok = false;
    }

    if (is_ok)
    {
        m_db_ptr = std::unique_ptr<SQLite::Database>(
            new SQLite::Database(
                m_db_path,
                SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE | SQLite::OPEN_NOFOLLOW));

        if (!m_db_ptr)
        {
            result.SetCode(DbCode::IO_ERROR);
            is_ok = false;
        }
    }

    if (is_ok)
    {
        ExecuteSql(DB_SQL_JOURNAL_MODE);
        ExecuteSql(DB_SQL_SYNCHRONOUS);
        ExecuteSql(DB_SQL_AUTO_VACUUM);
        ExecuteSql(DB_SQL_TEMP_STORE);

        int32_t cur_ver{0};
        const DbResult ver_ret = GetDBVersion(cur_ver);
        if (!ver_ret.IsOk())
        {
            result.SetCode(ver_ret.GetCode());
            is_ok = false;
        }
        else if (cur_ver < m_user_version && m_upgrade_callback)
        {
            const DbResult up_ret = m_upgrade_callback(cur_ver, m_user_version);
            if (!up_ret.IsOk())
            {
                result.SetCode(up_ret.GetCode());
                is_ok = false;
            }
            else
            {
                SetDBVersion(m_user_version);
            }
        }
    }

    if (is_ok)
    {
        result.SetCode(DbCode::OK);
    }

    return result;
}

DbResult DbAccess::GetDBVersion(int32_t& out_version)
{
    DbResult result{};
    bool is_ok{true};
    out_version = 0;

    if (is_ok && !m_db_ptr)
    {
        result.SetCode(DbCode::NOT_INIT);
        is_ok = false;
    }

    if (is_ok)
    {
        std::vector<std::map<std::string, std::string>> rows{};
        const DbResult ret = QuerySql(DB_SQL_PRAGMA_USER_VERSION, rows);
        if (!ret.IsOk() || rows.empty())
        {
            result.SetCode(ret.GetCode());
            is_ok = false;
        }
        else
        {
            const std::string& ver_str = rows[0]["user_version"];
            int32_t val = 0;
            if (!StrToInt32(ver_str.c_str(), val))
            {
                result.SetCode(DbCode::INVALID_DATA);
            }
            else
            {
                out_version = val;
                result.SetCode(DbCode::OK);
            }
        }
    }

    return result;
}

DbResult DbAccess::SetDBVersion(const int32_t version)
{
    DbResult result{};
    bool is_ok{true};

    if (is_ok && !m_db_ptr)
    {
        result.SetCode(DbCode::NOT_INIT);
        is_ok = false;
    }

    if (is_ok)
    {
        char sql[DB_SQL_BUF_SHORT] = {0};
        (void)snprintf(sql, sizeof(sql), "PRAGMA user_version = %" PRId32, version);
        result = ExecuteSql(sql);
    }

    return result;
}

DbResult DbAccess::ExecuteSql(const std::string& sql)
{
    return ExecuteSql(sql, {});
}

DbResult DbAccess::ExecuteSql(const std::string& sql, const std::vector<std::string>& params)
{
    DbResult result{};
    bool is_ok{true};

    if (is_ok && !m_db_ptr)
    {
        result.SetCode(DbCode::NOT_INIT);
        is_ok = false;
    }

    if (is_ok && sql.empty())
    {
        result.SetCode(DbCode::INVALID_ARG);
        is_ok = false;
    }

    if (is_ok)
    {
        SQLite::Statement stmt(*m_db_ptr, sql);
        const int32_t sz = static_cast<int32_t>(params.size());
        for (int32_t i = 0; i < sz; ++i)
        {
            stmt.bind(static_cast<int>(i + 1), params[i]);
        }

        const int32_t ret = static_cast<int32_t>(stmt.tryExecuteStep());
        if (ret != SQLITE_OK && ret != SQLITE_DONE)
        {
            result.SetCode(DbCode::INVALID_SQL);
        }
        else
        {
            result.SetCode(DbCode::OK);
        }
    }

    return result;
}

DbResult DbAccess::QuerySql(const std::string& sql, std::vector<std::map<std::string, std::string>>& out_rows)
{
    return QuerySql(sql, {}, out_rows);
}

DbResult DbAccess::QuerySql(const std::string& sql, const std::vector<std::string>& params, std::vector<std::map<std::string, std::string>>& out_rows)
{
    DbResult result{};
    bool is_ok{true};
    out_rows.clear();

    if (is_ok && !m_db_ptr)
    {
        result.SetCode(DbCode::NOT_INIT);
        is_ok = false;
    }

    if (is_ok && sql.empty())
    {
        result.SetCode(DbCode::INVALID_ARG);
        is_ok = false;
    }

    if (is_ok)
    {
        SQLite::Statement stmt(*m_db_ptr, sql);
        const int32_t sz = static_cast<int32_t>(params.size());
        for (int32_t i = 0; i < sz; ++i)
        {
            stmt.bind(static_cast<int>(i + 1), params[i]);
        }

        const int32_t col_cnt = static_cast<int32_t>(stmt.getColumnCount());
        while (true)
        {
            const int32_t ret = static_cast<int32_t>(stmt.tryExecuteStep());
            if (ret == SQLITE_DONE)
            {
                break;
            }
            if (ret != SQLITE_ROW)
            {
                result.SetCode(DbCode::INVALID_SQL);
                is_ok = false;
                break;
            }

            std::map<std::string, std::string> row{};
            for (int32_t i = 0; i < col_cnt; ++i)
            {
                const std::string name = stmt.getColumnName(i);
                const std::string val = stmt.getColumn(i).getString();
                row[name] = val;
            }
            out_rows.push_back(row);
        }
    }

    if (is_ok)
    {
        result.SetCode(DbCode::OK);
    }

    return result;
}

DbResult DbAccess::BeginTransaction()
{
    return ExecuteSql(DB_SQL_BEGIN_TRANSACTION);
}

DbResult DbAccess::Commit()
{
    return ExecuteSql(DB_SQL_COMMIT);
}

DbResult DbAccess::Rollback()
{
    return ExecuteSql(DB_SQL_ROLLBACK);
}

DbResult DbAccess::QueryPageUniversal(const std::string& sql_main,
                                      const std::vector<std::string>& params,
                                      const PageQuery& page_query,
                                      PageResult& out_result)
{
    DbResult result{};
    bool is_ok{true};
    out_result = PageResult{};

    if (is_ok && !m_db_ptr)
    {
        result.SetCode(DbCode::NOT_INIT);
        is_ok = false;
    }

    if (is_ok && (sql_main.empty() || page_query.sort_field.empty() || page_query.page_size <= 0))
    {
        result.SetCode(DbCode::INVALID_ARG);
        is_ok = false;
    }

    if (is_ok)
    {
        char sql_count[DB_SQL_BUF_NORMAL] = {0};
        (void)snprintf(sql_count, sizeof(sql_count), "SELECT COUNT(*) FROM (%s) AS t", sql_main.c_str());

        std::vector<std::map<std::string, std::string>> count_rows{};
        const DbResult count_ret = QuerySql(sql_count, params, count_rows);
        if (!count_ret.IsOk() || count_rows.empty())
        {
            result.SetCode(count_ret.GetCode());
            is_ok = false;
        }
        else
        {
            int32_t total = 0;
            const std::string& cnt_str = count_rows[0]["COUNT(*)"];
            if (!StrToInt32(cnt_str.c_str(), total))
            {
                result.SetCode(DbCode::INVALID_DATA);
                is_ok = false;
            }
            else
            {
                const int32_t total_page = (total + page_query.page_size - 1) / page_query.page_size;
                const int32_t offset = page_query.page_index * page_query.page_size;
                const char* ord = (page_query.order_type == DbOrderType::ASC) ? "ASC" : "DESC";

                char sql_data[DB_SQL_BUF_LONG] = {0};
                (void)snprintf(sql_data, sizeof(sql_data), "%s ORDER BY %s %s LIMIT ? OFFSET ?", sql_main.c_str(), page_query.sort_field.c_str(), ord);

                std::vector<std::string> page_params = params;
                page_params.push_back(std::to_string(page_query.page_size));
                page_params.push_back(std::to_string(offset));

                std::vector<std::map<std::string, std::string>> data_rows{};
                const DbResult data_ret = QuerySql(sql_data, page_params, data_rows);
                if (!data_ret.IsOk())
                {
                    result.SetCode(data_ret.GetCode());
                }
                else
                {
                    out_result.rows = std::move(data_rows);
                    out_result.total_count = total;
                    out_result.total_page = total_page;
                    out_result.page_index = page_query.page_index;
                    out_result.page_size = page_query.page_size;
                    result.SetCode(DbCode::OK);
                }
            }
        }
    }

    return result;
}

}  // namespace shortcut