#include "JobScDao.h"
#include "JobScDbAccess.h"
#include "JobScDbPageQuery.h"
#include "JobScLogger.h"
#include "JobScSqlDefines.h"

JobScDao::JobScDao()
  : JobScDao{nullptr}
{
}

JobScDao::JobScDao(const std::shared_ptr<JobScDbAccess>& db_ptr)
  : m_db_ptr{db_ptr}
{
}

bool JobScDao::Insert(uint64_t account_id, const JobScRow& row_data, int64_t& out_id)
{
    bool result = false;
    do
    {
        if (!m_db_ptr)
        {
            JOBSC_LOG_ERROR("JobScDao::Insert - database not initialized");
            break;
        }
        const std::vector<std::string> required_fields = {
            kFieldJobType,
            kFieldDescription,
            kFieldSettings,
            kFieldAddressList};
        if (!CheckRequiredFields(row_data, required_fields))
        {
            JOBSC_LOG_ERROR("JobScDao::Insert - missing required fields");
            break;
        }
        std::string sql = "INSERT INTO " + std::string(kTableName) +
                          " (" +
                          kFieldAccountId + ", " +
                          kFieldJobType + ", " +
                          kFieldDescription + ", " +
                          kFieldSettings + ", " +
                          kFieldAddressList +
                          ") VALUES (?, ?, ?, ?, ?)";
        std::vector<JobScValue> params;
        params.reserve(6);
        params.emplace_back(account_id);
        for (size_t i = 0; i < required_fields.size(); ++i)
        {
            const std::string& field = required_fields[i];
            params.push_back(row_data.at(field));
        }
        result = m_db_ptr->ExecuteSql(sql, params);
        if (!result)
        {
            JOBSC_LOG_ERROR("JobScDao::Insert - execute sql failed");
            break;
        }
        out_id = m_db_ptr->GetLastInsertRowId();
    } while (false);
    return result;
}

bool JobScDao::Delete(int64_t rid)
{
    bool result{false};
    if (m_db_ptr)
    {
        std::string sql = "DELETE FROM " + std::string(kTableName) + " WHERE " + kFieldRid + "=?";
        std::vector<JobScValue> params;
        params.emplace_back(rid);
        result = m_db_ptr->ExecuteSql(sql, params);
        if (!result)
        {
            JOBSC_LOG_ERROR("JobScDao::Delete - execute sql failed");
        }
    }
    else
    {
        JOBSC_LOG_ERROR("JobScDao::Delete - database not initialized");
    }
    return result;
}

bool JobScDao::DeleteByType(JobScType type)
{
    bool result{false};
    if (m_db_ptr)
    {
        std::string sql = "DELETE FROM " + std::string(kTableName) + " WHERE " + kFieldJobType + "=?";
        std::vector<JobScValue> params;
        params.emplace_back(static_cast<int64_t>(type));
        result = m_db_ptr->ExecuteSql(sql, params);
        if (!result)
        {
            JOBSC_LOG_ERROR("JobScDao::DeleteByType - execute sql failed");
        }
    }
    else
    {
        JOBSC_LOG_ERROR("JobScDao::DeleteByType - database not initialized");
    }
    return result;
}

bool JobScDao::Update(int64_t rid, const JobScRow& row_data)
{
    bool result{false};
    do
    {
        if (!m_db_ptr)
        {
            JOBSC_LOG_ERROR("JobScDao::Update - database not initialized");
            break;
        }
        const std::vector<std::string> required_fields = {
            kFieldJobType,
            kFieldDescription,
            kFieldSettings,
            kFieldAddressList};
        if (!CheckRequiredFields(row_data, required_fields))
        {
            JOBSC_LOG_ERROR("JobScDao::Update - missing required fields");
            break;
        }
        std::string sql = "UPDATE " + std::string(kTableName) + " SET " +
                          kFieldJobType + "=?, " +
                          kFieldDescription + "=?, " +
                          kFieldSettings + "=?, " +
                          kFieldAddressList + "=? " +
                          "WHERE " + kFieldRid + "=?";

        std::vector<JobScValue> params;
        params.reserve(5);
        for (size_t i = 0; i < required_fields.size(); ++i)
        {
            const std::string& field = required_fields[i];
            params.push_back(row_data.at(field));
        }
        params.emplace_back(rid);
        result = m_db_ptr->ExecuteSql(sql, params);
        if (!result)
        {
            JOBSC_LOG_ERROR("JobScDao::Update - execute sql failed");
        }
    } while (false);
    return result;
}

bool JobScDao::CheckRequiredFields(
    const JobScRow& row_data,
    const std::vector<std::string>& required_fields)
{
    bool all_fields_exist = true;
    for (size_t i = 0; i < required_fields.size(); ++i)
    {
        const std::string& field = required_fields[i];
        if (row_data.find(field) == row_data.cend())
        {
            JOBSC_LOG_ERROR("JobScDao::CheckRequiredFields - missing required field: %s", field.c_str());
            all_fields_exist = false;
            break;
        }
    }
    return all_fields_exist;
}

bool JobScDao::Query(const std::string& keyword, JobScType type, const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list)
{
    bool result{false};

    if (m_db_ptr)
    {
        if (type == JobScType::All)
        {
            if (keyword.empty())
            {
                result = GetAllPage(page_query, out_result, out_list);
            }
            else
            {
                result = GetAllByKeywordPage(keyword, page_query, out_result, out_list);
            }
        }
        else
        {
            if (keyword.empty())
            {
                result = Query(type, page_query, out_result, out_list);
            }
            else
            {
                result = GetListByTypeAndKeywordPage(type, keyword, page_query, out_result, out_list);
            }
        }
    }
    else
    {
        JOBSC_LOG_ERROR("JobScDao::Query - database not initialized");
    }

    return result;
}

bool JobScDao::GetAllPage(const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list)
{
    bool result{false};
    // 无类型 + 无关键词 → 查询全部
    std::string sql_main = "SELECT " +
                           std::string(kFieldRid) + ", " +
                           kFieldJobType + ", " +
                           kFieldDescription + ", " +
                           kFieldSettings + ", " +
                           kFieldAddressList +
                           " FROM " +
                           std::string(kTableName);
    std::vector<JobScValue> params;
    result = m_db_ptr->QueryPageUniversal(sql_main, params, page_query, out_result, out_list);
    return result;
}

bool JobScDao::GetAllByKeywordPage(const std::string& keyword, const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list)
{
    bool result{false};
    // 无类型 + 有关键词 → 模糊匹配
    std::string sql_main = "SELECT " +
                           std::string(kFieldRid) + ", " +
                           kFieldJobType + ", " +
                           kFieldDescription + ", " +
                           kFieldSettings + ", " +
                           kFieldAddressList +
                           " FROM " +
                           std::string(kTableName) + " WHERE " + kFieldDescription + " LIKE ?";
    std::vector<JobScValue> params;
    params.emplace_back("%" + keyword + "%");
    result = m_db_ptr->QueryPageUniversal(sql_main, params, page_query, out_result, out_list);
    return result;
}

bool JobScDao::Query(JobScType type, const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list)
{
    bool result{false};
    // 有类型 + 无关键词
    std::string sql_main = "SELECT " +
                           std::string(kFieldRid) + ", " +
                           kFieldJobType + ", " +
                           kFieldDescription + ", " +
                           kFieldSettings + ", " +
                           kFieldAddressList +
                           " FROM " +
                           std::string(kTableName) + " WHERE " + kFieldJobType + "=?";
    std::vector<JobScValue> params;
    params.emplace_back(static_cast<int64_t>(type));
    result = m_db_ptr->QueryPageUniversal(sql_main, params, page_query, out_result, out_list);
    return result;
}

bool JobScDao::GetListByTypeAndKeywordPage(JobScType type, const std::string& keyword, const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list)
{
    bool result{false};
    // 有类型 + 有关键词
    std::string sql_main = "SELECT " +
                           std::string(kFieldRid) + ", " +
                           kFieldJobType + ", " +
                           kFieldDescription + ", " +
                           kFieldSettings + ", " +
                           kFieldAddressList +
                           " FROM " +
                           std::string(kTableName) + " WHERE " + kFieldJobType + "=? AND " + kFieldDescription + " LIKE ?";
    std::vector<JobScValue> params;
    params.emplace_back(static_cast<int64_t>(type));
    params.emplace_back("%" + keyword + "%");
    result = m_db_ptr->QueryPageUniversal(sql_main, params, page_query, out_result, out_list);
    return result;
}

uint64_t JobScDao::GetCountByType(uint64_t account_id, JobScType job_type)
{
    uint64_t count{0u};
    std::string sql = "SELECT COUNT(*) AS total FROM " + std::string(kTableName) + " WHERE " + kFieldJobType + "=?";
    std::vector<JobScValue> params;
    params.emplace_back(static_cast<int64_t>(job_type));
    JobScRowList count_rows;
    do
    {
        if (!m_db_ptr)
        {
            JOBSC_LOG_ERROR("JobScDao::GetCountByType - database not initialized");
            break;
        }
        if (!m_db_ptr->QuerySql(sql, params, count_rows))
        {
            JOBSC_LOG_ERROR("JobScDao::GetCountByType - execute sql failed");
            break;
        }
        if (count_rows.empty())
        {
            JOBSC_LOG_ERROR("JobScDao::GetCountByType - no count result");
            break;
        }
        bool result{false};
        count = count_rows[0]["total"].ToUint64(result);
        if (!result)
        {
            JOBSC_LOG_ERROR("JobScDao::GetCountByType - failed to parse count result");
            count = 0;
            break;
        }
    } while (false);
    return count;
}

void JobScDao::BeginTransaction()
{
    if (m_db_ptr)
    {
        m_db_ptr->BeginTransaction();
    }
}

void JobScDao::CommitTransaction()
{
    if (m_db_ptr)
    {
        m_db_ptr->CommitTransaction();
    }
}

void JobScDao::RollbackTransaction()
{
    if (m_db_ptr)
    {
        m_db_ptr->RollbackTransaction();
    }
}