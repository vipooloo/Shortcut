#include "DbAccess.h"
#include <climits>
#include <cstdint>
#include <sqlite3.h>

namespace shortcut {
bool DbAccess::StrToInt32(const char* str, int32_t& out_val)
{
    out_val = 0;
    if ((str == nullptr) || (*str == '\0'))
    {
        return false;
    }

    char* end = nullptr;
    const long val = std::strtol(str, &end, 10);
    if ((end == str) || (*end != '\0'))
    {
        return false;
    }

    if ((val < INT32_MIN) || (val > INT32_MAX))
    {
        return false;
    }

    out_val = static_cast<int32_t>(val);
    return true;
}

DbValue DbAccess::WrapColumnValue(const SQLite::Column& col)
{
    switch (col.getType())
    {
        case SQLITE_INTEGER:
            return DbValue(col.getInt64());
        case SQLITE_TEXT:
            return DbValue(col.getString());
        case SQLITE_BLOB:
            return DbValue(static_cast<const uint8_t*>(col.getBlob()),
                           static_cast<uint32_t>(col.getBytes()));
        default:
            return DbValue();
    }
}

DbAccess::DbAccess()
  : m_db_path()
  , m_user_version(0)
  , m_db_ptr(nullptr)
  , m_upgrade_callback()
{
}

DbResult DbAccess::Init()
{
    DbResult result;
    bool is_ok = true;

    if (m_db_path.empty())
    {
        result.SetCode(DbCode::INVALID_ARG);
        is_ok = false;
    }

    if (is_ok)
    {
        m_db_ptr = std::unique_ptr<SQLite::Database>(new SQLite::Database(m_db_path,
                                                                          SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE | SQLite::OPEN_NOFOLLOW));
        if (m_db_ptr == nullptr)
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
    }

    if (is_ok)
    {
        int32_t cur_ver = 0;
        DbResult ver_ret = GetDbVersion(cur_ver);
        if (!ver_ret.IsOk())
        {
            result.SetCode(ver_ret.GetCode());
        }
        else if ((cur_ver < m_user_version) && (m_upgrade_callback != nullptr))
        {
            DbResult up_ret = m_upgrade_callback(cur_ver, m_user_version);
            if (!up_ret.IsOk())
            {
                result.SetCode(up_ret.GetCode());
            }
            else
            {
                SetDbVersion(m_user_version);
            }
        }
    }

    if (is_ok)
    {
        result.SetCode(DbCode::OK);
    }
    return result;
}

DbResult DbAccess::GetDbVersion(int32_t& out_version)
{
    DbResult result;
    bool is_ok = true;
    out_version = 0;

    if (m_db_ptr == nullptr)
    {
        result.SetCode(DbCode::NOT_INIT);
        is_ok = false;
    }

    DbRows rows;
    if (is_ok)
    {
        DbResult ret = QuerySql(DB_SQL_PRAGMA_USER_VERSION, rows);
        if ((!ret.IsOk()) || (rows.empty()))
        {
            result.SetCode(ret.GetCode());
            is_ok = false;
        }
    }

    if (is_ok)
    {
        bool ok = false;
        std::string ver_str = rows[0].at("user_version").ToString(ok);
        if ((!ok) || (!StrToInt32(ver_str.c_str(), out_version)))
        {
            result.SetCode(DbCode::INVALID_DATA);
        }
        else
        {
            result.SetCode(DbCode::OK);
        }
    }
    return result;
}

DbResult DbAccess::SetDbVersion(int32_t version)
{
    DbResult result;
    bool is_ok = true;

    if (m_db_ptr == nullptr)
    {
        result.SetCode(DbCode::NOT_INIT);
        is_ok = false;
    }

    if (is_ok)
    {
        char sql[DB_SQL_BUF_SHORT] = {0};
        (void)snprintf(sql, sizeof(sql), "PRAGMA user_version = %d", version);
        result = ExecuteSql(sql);
    }
    return result;
}

DbResult DbAccess::ExecuteSql(const std::string& sql)
{
    return ExecuteSql(sql, std::vector<std::string>());
}

DbResult DbAccess::ExecuteSql(const std::string& sql, const std::vector<std::string>& params)
{
    DbResult result;
    bool is_ok = true;

    if (m_db_ptr == nullptr)
    {
        result.SetCode(DbCode::NOT_INIT);
        is_ok = false;
    }
    if (sql.empty())
    {
        result.SetCode(DbCode::INVALID_ARG);
        is_ok = false;
    }

    if (is_ok)
    {
        SQLite::Statement stmt(*m_db_ptr, sql);
        int32_t sz = static_cast<int32_t>(params.size());
        for (int32_t i = 0; i < sz; i++)
        {
            stmt.bind(static_cast<int>(i + 1), params[i]);
        }

        int32_t ret = static_cast<int32_t>(stmt.tryExecuteStep());
        if ((ret != SQLITE_OK) && (ret != SQLITE_DONE))
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

DbResult DbAccess::QuerySql(const std::string& sql, DbRows& out_rows)
{
    return QuerySql(sql, std::vector<std::string>(), out_rows);
}

DbResult DbAccess::QuerySql(const std::string& sql,
                            const std::vector<std::string>& params,
                            DbRows& out_rows)
{
    DbResult result;
    bool is_ok = true;
    out_rows.clear();

    if (m_db_ptr == nullptr)
    {
        result.SetCode(DbCode::NOT_INIT);
        is_ok = false;
    }
    if (sql.empty())
    {
        result.SetCode(DbCode::INVALID_ARG);
        is_ok = false;
    }

    if (is_ok)
    {
        SQLite::Statement stmt(*m_db_ptr, sql);
        int32_t sz = static_cast<int32_t>(params.size());
        for (int32_t i = 0; i < sz; i++)
        {
            stmt.bind(static_cast<int>(i + 1), params[i]);
        }

        int32_t col_cnt = static_cast<int32_t>(stmt.getColumnCount());
        while (true)
        {
            int32_t ret = static_cast<int32_t>(stmt.tryExecuteStep());
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

            DbRow row;
            for (int32_t i = 0; i < col_cnt; i++)
            {
                std::string name = stmt.getColumnName(i);
                row[name] = WrapColumnValue(stmt.getColumn(i));
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

void DbAccess::SetDbPath(const std::string& path)
{
    m_db_path = path;
}

void DbAccess::SetUserVersion(int32_t version)
{
    m_user_version = version;
}

void DbAccess::SetUpgradeCallback(const UpgradeCallback& cb)
{
    m_upgrade_callback = cb;
}

DbResult DbAccess::QueryTotalCount(const std::string& sql_main,
                                   const std::vector<std::string>& params,
                                   int32_t& total)
{
    DbResult result;
    total = 0;
    char sql_count[DB_SQL_BUF_NORMAL] = {0};

    (void)snprintf(sql_count, sizeof(sql_count), "SELECT COUNT(*) FROM (%s) AS t", sql_main.c_str());
    DbRows count_rows;
    result = QuerySql(sql_count, params, count_rows);
    if ((!result.IsOk()) || (count_rows.empty()))
    {
        return result;
    }

    bool ok = false;
    std::string cnt_str = count_rows[0].at("COUNT(*)").ToString(ok);
    if ((!ok) || (!StrToInt32(cnt_str.c_str(), total)))
    {
        result.SetCode(DbCode::INVALID_DATA);
    }
    return result;
}

DbResult DbAccess::QueryPageData(const std::string& sql_main,
                                 const std::vector<std::string>& params,
                                 const PageQuery& page_query,
                                 int32_t total,
                                 PageResult& out_result,
                                 DbRows& out_rows)
{
    DbResult result;
    out_rows.clear();

    int32_t total_page = (total + page_query.page_size - 1) / page_query.page_size;
    int32_t offset = page_query.page_index * page_query.page_size;
    const char* ord = (DbOrderType::ASC == page_query.order_type) ? "ASC" : "DESC";

    char sql_data[DB_SQL_BUF_LONG] = {0};
    (void)snprintf(sql_data, sizeof(sql_data), "%s ORDER BY %s %s LIMIT ? OFFSET ?", sql_main.c_str(), page_query.sort_field.c_str(), ord);

    std::vector<std::string> page_params = params;
    page_params.push_back(std::to_string(page_query.page_size));
    page_params.push_back(std::to_string(offset));

    result = QuerySql(sql_data, page_params, out_rows);
    if (result.IsOk())
    {
        out_result.total_count = total;
        out_result.total_page = total_page;
        out_result.page_index = page_query.page_index;
        out_result.page_size = page_query.page_size;
    }
    return result;
}

DbResult DbAccess::QueryPageUniversal(const std::string& sql_main,
                                      const std::vector<std::string>& params,
                                      const PageQuery& page_query,
                                      PageResult& out_result,
                                      DbRows& out_rows)
{
    DbResult result;
    bool is_ok = true;

    out_result.total_count = 0;
    out_result.total_page = 0;
    out_result.page_index = 0;
    out_result.page_size = 0;
    out_rows.clear();

    if (m_db_ptr == nullptr)
    {
        result.SetCode(DbCode::NOT_INIT);
        is_ok = false;
    }
    if ((sql_main.empty()) || (page_query.sort_field.empty()) || (page_query.page_size <= 0))
    {
        result.SetCode(DbCode::INVALID_ARG);
        is_ok = false;
    }

    if (is_ok)
    {
        int32_t total = 0;
        result = QueryTotalCount(sql_main, params, total);
        if (!result.IsOk())
        {
            is_ok = false;
        }
        else
        {
            result = QueryPageData(sql_main, params, page_query, total, out_result, out_rows);
        }
    }
    return result;
}
}  // namespace shortcut