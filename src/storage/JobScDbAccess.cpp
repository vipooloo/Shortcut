#include "JobScDbAccess.h"
#include "JobScLogger.h"
#include "JobScSqlDefines.h"
#include <cinttypes>
#include <climits>
#include <cstdint>
#include <sqlite3.h>

JobScDbAccess::JobScDbAccess()
  : m_db_path()
  , m_user_version(0)
  , m_db_ptr(nullptr)
  , m_upgrade_callback()
{
}

JobScValue JobScDbAccess::WrapColumnValue(const SQLite::Column& col)
{
    JobScValue value{};

    int32_t type = static_cast<int32_t>(col.getType());
    switch (type)
    {
        case SQLITE_INTEGER:
            value = JobScValue(col.getInt64());
            break;
        case SQLITE_TEXT:
            value = JobScValue(col.getString());
            break;
        case SQLITE_BLOB:
            value = JobScValue(static_cast<const uint8_t*>(col.getBlob()),
                               static_cast<uint32_t>(col.getBytes()));
            break;
        default:
            JOBSC_LOG_ERROR("JobScDbAccess::WrapColumnValue - unsupported column type:%d", type);
            break;
    }

    return value;
}

bool JobScDbAccess::Init()
{
    JOBSC_LOG_FORCE("JobScDbAccess::Init() - db_path:%s %" PRId64, m_db_path.c_str(), m_user_version);

    bool result{false};
    do
    {
        if (m_db_path.empty())
        {
            JOBSC_LOG_ERROR("JobScDbAccess::Init() - db_path is empty");
            break;
        }
        m_db_ptr = std::unique_ptr<SQLite::Database>(new SQLite::Database(m_db_path,
                                                                          SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE | SQLite::OPEN_NOFOLLOW));
        if (!m_db_ptr)
        {
            JOBSC_LOG_ERROR("JobScDbAccess::Init() - failed to create database instance");
            break;
        }
        if (!m_upgrade_callback)
        {
            JOBSC_LOG_ERROR("JobScDbAccess::Init() - upgrade callback is null");
            break;
        }
        BeginTransaction();
        const std::vector<std::string> pragmas = {
            DB_SQL_FOREIGN_KEYS,  ///<
            DB_SQL_JOURNAL_MODE,  ///<
            DB_SQL_SYNCHRONOUS,   ///<
            DB_SQL_AUTO_VACUUM,   ///<
            DB_SQL_TEMP_STORE     ///<
        };
        for (const std::string& pragma : pragmas)
        {
            if (ExecuteSql(pragma))
            {
                break;
            }
        }
        int64_t cur_ver = 0;
        if (!GetDbVersion(cur_ver))
        {
            JOBSC_LOG_ERROR("JobScDbAccess::Init() - failed to get db version");
            break;
        }
        JOBSC_LOG_FORCE("JobScDbAccess::Init() - upgrade db from version %ld to %ld", cur_ver, m_user_version);
        if (cur_ver < m_user_version)
        {
            result = m_upgrade_callback(cur_ver, m_user_version);
            if (!result || (!SetDbVersion(m_user_version)))
            {
                result = false;
                JOBSC_LOG_ERROR("JobScDbAccess::Init() - failed to set db version");
            }
        }
        else if (cur_ver > m_user_version)
        {
            JOBSC_LOG_ERROR("JobScDbAccess::Init() - db version is up to date");
            break;
        }
        else
        {
        }
        result = true;
    } while (false);
    if (result)
    {
        JOBSC_LOG_FORCE("JobScDbAccess::Init() - db init success");
        CommitTransaction();
    }
    else
    {
        JOBSC_LOG_ERROR("JobScDbAccess::Init() - db init failed");
        RollbackTransaction();
    }
    return result;
}

bool JobScDbAccess::GetDbVersion(int64_t& out_version)
{
    bool result{false};
    JobScRowList rows;
    result = QuerySql(DB_SQL_PRAGMA_USER_VERSION, rows);
    if (result && !rows.empty())
    {
        out_version = rows[0]["user_version"].ToInt64(result);
    }
    return result;
}

bool JobScDbAccess::SetDbVersion(int64_t version)
{
    char sql[DB_SQL_BUFFER] = {0};
    snprintf(sql, sizeof(sql), "PRAGMA user_version = %" PRId64, version);
    return ExecuteSql(sql);
}

bool JobScDbAccess::ExecuteSql(const std::string& sql)
{
    return ExecuteSql(sql, std::vector<JobScValue>());
}

bool JobScDbAccess::ExecuteSql(const std::string& sql, const std::vector<JobScValue>& params)
{
    bool result{false};
    if (m_db_ptr && !sql.empty())
    {
        SQLite::Statement stmt(*m_db_ptr, sql);
        for (size_t i = 0; i < params.size(); ++i)
        {
            int32_t idx = static_cast<int32_t>(i + 1);
            const JobScValue& param = params[i];

            switch (param.GetType())
            {
                case JobScValType::Int:
                {
                    int64_t val = param;
                    stmt.bind(idx, val);
                    break;
                }
                case JobScValType::String:
                {
                    std::string val = param;
                    stmt.bind(idx, val);
                    break;
                }
                case JobScValType::Blob:
                {
                    const std::vector<uint8_t>& data = param;
                    stmt.bind(idx, data.data(), data.size());
                    break;
                }
                default:
                    JOBSC_LOG_ERROR("JobScDbAccess::ExecuteSql %s - unsupported param type:%d", sql.c_str(), static_cast<int32_t>(param.GetType()));
                    break;
            }
        }

        int32_t ret = static_cast<int32_t>(stmt.tryExecuteStep());
        if ((ret != SQLITE_OK) && (ret != SQLITE_DONE) && (ret != SQLITE_ROW))
        {
            JOBSC_LOG_ERROR("JobScDbAccess::ExecuteSql %s - execute failed, ret:%d", sql.c_str(), ret);
        }
        else
        {
            result = true;
        }
    }
    else
    {
        JOBSC_LOG_ERROR("JobScDbAccess::ExecuteSql %s - db not init", sql.c_str());
    }
    return result;
}

bool JobScDbAccess::QuerySql(const std::string& sql, JobScRowList& out_rows)
{
    return QuerySql(sql, std::vector<JobScValue>(), out_rows);
}

int64_t JobScDbAccess::GetLastInsertRowId()
{
    return m_db_ptr ? m_db_ptr->getLastInsertRowid() : 0;
}

bool JobScDbAccess::QuerySql(const std::string& sql, const std::vector<JobScValue>& params, JobScRowList& out_rows)
{
    bool result{false};
    if (m_db_ptr && !sql.empty() && out_rows.empty())
    {
        SQLite::Statement stmt(*m_db_ptr, sql);
        for (size_t i = 0; i < params.size(); ++i)
        {
            int32_t idx = static_cast<int32_t>(i + 1);
            const JobScValue& param = params[i];

            switch (param.GetType())
            {
                case JobScValType::Int:
                {
                    int64_t val = param;
                    stmt.bind(idx, val);
                    break;
                }
                case JobScValType::String:
                {
                    std::string val = param;
                    stmt.bind(idx, val);
                    break;
                }
                case JobScValType::Blob:
                {
                    const std::vector<uint8_t>& data = param;
                    stmt.bind(idx, data.data(), data.size());
                    break;
                }
                default:
                    JOBSC_LOG_ERROR("JobScDbAccess::QuerySql %s - unsupported param type:%d", sql.c_str(), static_cast<int32_t>(param.GetType()));
                    break;
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
                JOBSC_LOG_ERROR("JobScDbAccess::QuerySql %s - execute failed, ret:%d", sql.c_str(), ret);
                out_rows.clear();
                result = false;
                break;
            }

            JobScRow row;
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
        JOBSC_LOG_ERROR("JobScDbAccess::QuerySql %s - db not init", sql.c_str());
    }
    return result;
}

void JobScDbAccess::BeginTransaction()
{
    ExecuteSql(DB_SQL_BEGIN_TRANSACTION);
}

void JobScDbAccess::CommitTransaction()
{
    ExecuteSql(DB_SQL_COMMIT_TRANSACTION);
}

void JobScDbAccess::RollbackTransaction()
{
    ExecuteSql(DB_SQL_ROLLBACK_TRANSACTION);
}

bool JobScDbAccess::QueryTotalCount(const std::string& sql_main,
                                    const std::vector<JobScValue>& params,
                                    uint32_t& total)
{
    bool result{false};
    char sql_count[DB_SQL_BUFFER] = {0};

    snprintf(sql_count, sizeof(sql_count), "SELECT COUNT(*) AS total FROM (%s) AS t", sql_main.c_str());
    JobScRowList count_rows;
    result = QuerySql(sql_count, params, count_rows);
    if (result && !count_rows.empty())
    {
        int64_t ver = count_rows[0]["total"].ToInt64(result);
        if (result)
        {
            total = static_cast<uint32_t>(ver);
        }
    }
    return result;
}

bool JobScDbAccess::QueryPageData(const std::string& sql_main,
                                  const std::vector<JobScValue>& params,
                                  const JobScDbPageQuery& page_query,
                                  uint32_t total,
                                  JobScPageResult& out_result,
                                  JobScRowList& out_rows)
{
    bool result{false};
    uint32_t page_size{page_query.GetPageSize()};

    if (page_size > 1)
    {
        uint32_t total_page = (total + page_size - 1) / page_size;
        uint32_t offset = page_query.GetPageIndex() * page_size;
        const char* ord = (JobScOrderType::ASC == page_query.GetOrderType()) ? "ASC" : "DESC";

        char sql_data[DB_SQL_BUFFER] = {0};
        snprintf(sql_data, sizeof(sql_data), "%s ORDER BY %s %s LIMIT ? OFFSET ?", sql_main.c_str(), page_query.GetSortFieldCStr(), ord);

        std::vector<JobScValue> page_params = params;
        page_params.emplace_back(JobScValue(std::to_string(page_size)));
        page_params.emplace_back(JobScValue(std::to_string(offset)));

        if (QuerySql(sql_data, page_params, out_rows))
        {
            result = true;
            out_result.total_count = total;
            out_result.total_page = total_page;
            out_result.page_index = page_query.GetPageIndex();
            out_result.page_size = page_size;
        }
        else
        {
            JOBSC_LOG_ERROR("JobScDbAccess::QueryPageData - invalid page size:%u", page_size);
        }
    }
    return result;
}

bool JobScDbAccess::QueryPageUniversal(const std::string& sql_main,
                                       const std::vector<JobScValue>& params,
                                       const JobScDbPageQuery& page_query,
                                       JobScPageResult& out_result,
                                       JobScRowList& out_rows)
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
