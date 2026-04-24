#include "DbAccess.h"
#include <cinttypes>
#include <cstdint>
#include <sqlite3.h>
#include <stdexcept>
#include <string>

#define LOG_ERROR(...)
#define LOG_FORCE_ERROR(...)

namespace shortcut {

DbResult DbAccess::Init()
{
    DbResult result{};
    bool is_ok{true};

    if (is_ok && m_db_path.empty())
    {
        LOG_ERROR("Init: empty db path");
        result.SetCode(DbCode::INVALID_ARG);
        is_ok = false;
    }

    if (is_ok)
    {
        try
        {
            m_db_ptr.reset(new SQLite::Database(m_db_path,
                                                SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE | SQLite::OPEN_NOFOLLOW));
        }
        catch (...)
        {
            LOG_ERROR("Init: open failed");
            result.SetCode(DbCode::IO_ERROR);
            is_ok = false;
        }
    }

    if (is_ok && !m_db_ptr)
    {
        LOG_ERROR("Init: null ptr");
        result.SetCode(DbCode::NOT_INIT);
        is_ok = false;
    }

    if (is_ok)
    {
        ExecuteSql("PRAGMA journal_mode = DELETE;");
        ExecuteSql("PRAGMA synchronous = FULL;");
        ExecuteSql("PRAGMA auto_vacuum = FULL;");
        ExecuteSql("PRAGMA temp_store = MEMORY;");

        int32_t cur_ver{0};
        DbResult ver_ret{GetDBVersion(cur_ver)};
        if (!ver_ret.IsOk())
        {
            result.SetCode(ver_ret.GetCode());
            is_ok = false;
        }
        else if (cur_ver < m_user_version && m_upgrade_callback)
        {
            DbResult up_ret{m_upgrade_callback(cur_ver, m_user_version)};
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
        DbResult ret{QuerySql("PRAGMA user_version;", rows)};
        if (!ret.IsOk() || rows.empty())
        {
            result.SetCode(ret.GetCode());
            is_ok = false;
        }
        else
        {
            out_version = std::stoi(rows[0]["user_version"]);
            result.SetCode(DbCode::OK);
        }
    }

    return result;
}

DbResult DbAccess::SetDBVersion(int32_t version)
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
        char sql[64]{};
        snprintf(sql, sizeof(sql), "PRAGMA user_version = %" PRId32, version);
        DbResult ret{ExecuteSql(sql)};
        result = ret;
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
        SQLite::Statement stmt{*m_db_ptr, sql};
        for (int32_t i{0}; i < static_cast<int32_t>(params.size()); ++i)
        {
            stmt.bind(static_cast<int>(i + 1), params[i]);
        }

        int32_t ret{static_cast<int32_t>(stmt.tryExecuteStep())};
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
        SQLite::Statement stmt{*m_db_ptr, sql};
        for (int32_t i{0}; i < static_cast<int32_t>(params.size()); ++i)
        {
            stmt.bind(static_cast<int>(i + 1), params[i]);
        }

        int32_t col_count{static_cast<int32_t>(stmt.getColumnCount())};
        while (true)
        {
            int32_t ret{static_cast<int32_t>(stmt.tryExecuteStep())};
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
            for (int32_t i{0}; i < col_count; ++i)
            {
                row[stmt.getColumnName(i)] = stmt.getColumn(i).getString();
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
    return ExecuteSql("BEGIN TRANSACTION;");
}

DbResult DbAccess::Commit()
{
    return ExecuteSql("COMMIT;");
}

DbResult DbAccess::Rollback()
{
    return ExecuteSql("ROLLBACK;");
}

// ==============================
// 最终版：独立分页类 + 单return
// ==============================
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
        char sql_count[512]{};
        snprintf(sql_count, sizeof(sql_count), "SELECT COUNT(*) FROM (%s) AS t", sql_main.c_str());

        std::vector<std::map<std::string, std::string>> count_rows{};
        DbResult count_ret{QuerySql(sql_count, params, count_rows)};

        if (!count_ret.IsOk() || count_rows.empty())
        {
            result.SetCode(count_ret.GetCode());
            is_ok = false;
        }
        else
        {
            int32_t total_count{0};
            try
            {
                total_count = std::stoi(count_rows[0]["COUNT(*)"]);
            }
            catch (...)
            {
                result.SetCode(DbCode::INVALID_DATA);
                is_ok = false;
            }

            if (is_ok)
            {
                int32_t total_page{(total_count + page_query.page_size - 1) / page_query.page_size};
                int32_t offset{page_query.page_index * page_query.page_size};
                const char* ord = page_query.order_type == DbOrderType::ASC ? "ASC" : "DESC";

                char sql_data[1024]{};
                snprintf(sql_data, sizeof(sql_data), "%s ORDER BY %s %s LIMIT ? OFFSET ?", sql_main.c_str(), page_query.sort_field.c_str(), ord);

                std::vector<std::string> page_params{params};
                page_params.push_back(std::to_string(page_query.page_size));
                page_params.push_back(std::to_string(offset));

                std::vector<std::map<std::string, std::string>> data_rows{};
                DbResult data_ret{QuerySql(sql_data, page_params, data_rows)};

                if (!data_ret.IsOk())
                {
                    result.SetCode(data_ret.GetCode());
                }
                else
                {
                    out_result.rows = std::move(data_rows);
                    out_result.total_count = total_count;
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