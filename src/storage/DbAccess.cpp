#include "DbAccess.h"
#include "ShortcutLogger.h"
#include <climits>
#include <cstdint>
#include <sqlite3.h>

namespace shortcut {

DbAccess::DbAccess()
  : m_db_path()
  , m_user_version(0)
  , m_db_ptr(nullptr)
  , m_upgrade_callback()
{
}

DbValue DbAccess::WrapColumnValue(const SQLite::Column& col)
{
    DbValue value{};

    int32_t type = static_cast<int32_t>(col.getType());
    switch (type)
    {
        case SQLITE_INTEGER:
            value = DbValue(col.getInt64());
            break;
        case SQLITE_TEXT:
            value = DbValue(col.getString());
            break;
        case SQLITE_BLOB:
            value = DbValue(static_cast<const uint8_t*>(col.getBlob()),
                            static_cast<uint32_t>(col.getBytes()));
            break;
        default:
            LOG_ERROR("DbAccess::WrapColumnValue - unsupported column type:%d", type);
            break;
    }

    return value;
}

bool DbAccess::Init()
{
    LOG_FORCE("DbAccess::Init() - db_path:%s %ld", m_db_path.c_str(), m_user_version);

    bool result{false};
    do
    {
        if (m_db_path.empty())
        {
            LOG_ERROR("DbAccess::Init() - db_path is empty");
            break;
        }
        m_db_ptr = std::unique_ptr<SQLite::Database>(new SQLite::Database(m_db_path,
                                                                          SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE | SQLite::OPEN_NOFOLLOW));
        if (!m_db_ptr)
        {
            LOG_ERROR("DbAccess::Init() - failed to create database instance");
            break;
        }
        BeginTransaction();
        const std::vector<std::string> pragmas = {DB_SQL_JOURNAL_MODE, DB_SQL_SYNCHRONOUS, DB_SQL_AUTO_VACUUM, DB_SQL_TEMP_STORE};
        for (const std::string& pragma : pragmas)
        {
            if (ExecuteSql(pragma))
            {
                break;
            }
        }
        int64_t cur_ver = 0;
        if (GetDbVersion(cur_ver))
        {
            LOG_ERROR("DbAccess::Init() - failed to get db version");
            break;
        }
        LOG_FORCE("DbAccess::Init() - upgrade db from version %ld to %ld", cur_ver, m_user_version);
        if ((cur_ver < m_user_version) && m_upgrade_callback)
        {
            result = m_upgrade_callback(cur_ver, m_user_version);
            if (!result || (!SetDbVersion(m_user_version)))
            {
                result = false;
                LOG_ERROR("DbAccess::Init() - failed to set db version");
            }
            else
            {
                LOG_ERROR("DbAccess::Init() - upgrade failed");
                break;
            }
        }
    } while (false);
    if (result)
    {
        LOG_FORCE("DbAccess::Init() - db init success");
        CommitTransaction();
    }
    else
    {
        LOG_ERROR("DbAccess::Init() - db init failed");
        RollbackTransaction();
    }
    return result;
}

bool DbAccess::GetDbVersion(int64_t& out_version)
{
    bool result{false};
    DbRows rows;
    result = QuerySql(DB_SQL_PRAGMA_USER_VERSION, rows);
    if (result && !rows.empty())
    {
        out_version = rows[0].at("user_version").ToInt64(result);
    }
    return result;
}

bool DbAccess::SetDbVersion(int64_t version)
{
    char sql[DB_SQL_BUF_SHORT] = {0};
    snprintf(sql, sizeof(sql), "PRAGMA user_version = %ld", version);
    return ExecuteSql(sql);
}

bool DbAccess::ExecuteSql(const std::string& sql)
{
    return ExecuteSql(sql, std::vector<SqlParam>());
}

bool DbAccess::ExecuteSql(const std::string& sql, const std::vector<SqlParam>& params)
{
    bool result{false};
    if (m_db_ptr && !sql.empty())
    {
        SQLite::Statement stmt(*m_db_ptr, sql);
        for (size_t i = 0; i < params.size(); ++i)
        {
            int idx = static_cast<int>(i + 1);
            const SqlParam& param = params[i];
            const std::vector<uint8_t>& data = param.GetData();

            switch (param.GetType())
            {
                case SqlParam::Type::kInt32:
                {
                    int32_t val = *reinterpret_cast<const int32_t*>(data.data());
                    stmt.bind(idx, val);
                    break;
                }
                case SqlParam::Type::kInt64:
                {
                    int64_t val = *reinterpret_cast<const int64_t*>(data.data());
                    stmt.bind(idx, val);
                    break;
                }
                case SqlParam::Type::kString:
                {
                    std::string val(reinterpret_cast<const char*>(data.data()), data.size());
                    stmt.bind(idx, val);
                    break;
                }
            }
        }

        int32_t ret = static_cast<int32_t>(stmt.tryExecuteStep());
        if ((ret != SQLITE_OK) && (ret != SQLITE_DONE))
        {
            LOG_ERROR("DbAccess::ExecuteSql %s - execute failed, ret:%d", sql.c_str(), ret);
        }
        else
        {
            result = true;
        }
    }
    else
    {
        LOG_ERROR("DbAccess::ExecuteSql %s - db not init", sql.c_str());
    }
    return result;
}

bool DbAccess::QuerySql(const std::string& sql, DbRows& out_rows)
{
    return QuerySql(sql, std::vector<SqlParam>(), out_rows);
}

bool DbAccess::QuerySql(const std::string& sql, const std::vector<SqlParam>& params, DbRows& out_rows)
{
    bool result{false};
    if (m_db_ptr && !sql.empty() && out_rows.empty())
    {
        SQLite::Statement stmt(*m_db_ptr, sql);
        for (size_t i = 0; i < params.size(); ++i)
        {
            int idx = static_cast<int>(i + 1);
            const SqlParam& param = params[i];
            const std::vector<uint8_t>& data = param.GetData();

            switch (param.GetType())
            {
                case SqlParam::Type::kInt32:
                {
                    int32_t val = *reinterpret_cast<const int32_t*>(data.data());
                    stmt.bind(idx, val);
                    break;
                }
                case SqlParam::Type::kInt64:
                {
                    int64_t val = *reinterpret_cast<const int64_t*>(data.data());
                    stmt.bind(idx, val);
                    break;
                }
                case SqlParam::Type::kString:
                {
                    std::string val(reinterpret_cast<const char*>(data.data()), data.size());
                    stmt.bind(idx, val);
                    break;
                }
            }
        }

        int32_t col_cnt = static_cast<int32_t>(stmt.getColumnCount());
        while (true)
        {
            int32_t ret = static_cast<int32_t>(stmt.tryExecuteStep());
            if (ret == SQLITE_DONE)
            {
                result = true;
                break;
            }
            if (ret != SQLITE_ROW)
            {
                LOG_ERROR("DbAccess::QuerySql %s - execute failed, ret:%d", sql.c_str(), ret);
                out_rows.clear();
                result = false;
                break;
            }

            DbRow row;
            for (int32_t i = 0; i < col_cnt; ++i)
            {
                std::string name{stmt.getColumnName(i)};
                row[name] = WrapColumnValue(stmt.getColumn(i));
            }
            out_rows.push_back(row);
        }
    }
    else
    {
        LOG_ERROR("DbAccess::QuerySql %s - db not init", sql.c_str());
    }
    return result;
}

void DbAccess::BeginTransaction()
{
    ExecuteSql(DB_SQL_BEGIN_TRANSACTION);
}

void DbAccess::CommitTransaction()
{
    ExecuteSql(DB_SQL_COMMIT_TRANSACTION);
}

void DbAccess::RollbackTransaction()
{
    ExecuteSql(DB_SQL_ROLLBACK_TRANSACTION);
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

bool DbAccess::QueryTotalCount(const std::string& sql_main,
                               const std::vector<SqlParam>& params,
                               uint32_t& total)
{
    bool result{false};
    char sql_count[DB_SQL_BUF_NORMAL] = {0};

    snprintf(sql_count, sizeof(sql_count), "SELECT COUNT(*) FROM (%s) AS t", sql_main.c_str());
    DbRows count_rows;
    result = QuerySql(sql_count, params, count_rows);
    if (result && !count_rows.empty())
    {
        int64_t ver_str = count_rows[0].at("COUNT(*)").ToInt64(result);
        if (result)
        {
            total = static_cast<uint32_t>(ver_str);
        }
    }
    return result;
}

bool DbAccess::QueryPageData(const std::string& sql_main,
                             const std::vector<SqlParam>& params,
                             const PageQuery& page_query,
                             uint32_t total,
                             PageResult& out_result,
                             DbRows& out_rows)
{
    bool result{false};
    uint32_t page_size{page_query.GetPageSize()};

    if (page_size > 1)
    {
        uint32_t total_page = (total + page_size - 1) / page_size;
        uint32_t offset = page_query.GetPageIndex() * page_size;
        const char* ord = (DbOrderType::ASC == page_query.GetOrderType()) ? "ASC" : "DESC";

        char sql_data[DB_SQL_BUF_LONG] = {0};
        snprintf(sql_data, sizeof(sql_data), "%s ORDER BY %s %s LIMIT ? OFFSET ?", sql_main.c_str(), page_query.GetSortFieldCStr(), ord);

        std::vector<SqlParam> page_params = params;
        page_params.emplace_back(SqlParam(std::to_string(page_size)));
        page_params.emplace_back(SqlParam(std::to_string(offset)));

        if (QuerySql(sql_data, page_params, out_rows))
        {
            result = true;
            out_result =
                {
                    total,
                    total_page,
                    page_query.GetPageIndex(),
                    page_size};
        }
    }
    else
    {
        LOG_ERROR("DbAccess::QueryPageData - invalid page size:%u", page_size);
    }

    return result;
}

bool DbAccess::QueryPageUniversal(const std::string& sql_main,
                                  const std::vector<SqlParam>& params,
                                  const PageQuery& page_query,
                                  PageResult& out_result,
                                  DbRows& out_rows)
{
    bool result{false};
    if (m_db_ptr && (!sql_main.empty()) && (!page_query.GetSortField().empty()) && out_rows.empty())
    {
        uint32_t total = 0;
        if (QueryTotalCount(sql_main, params, total))
        {
            result = QueryPageData(sql_main, params, page_query, total, out_result, out_rows);
        }
    }
    return result;
}

}  // namespace shortcut