#include "JobScDao.h"
#include "JobScLogger.h"

// 表名
constexpr const char* kTableName = "shortcut";

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
        std::vector<JobScDbValue> params;
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
        std::string sql = "DELETE FROM " + std::string(kTableName) + " WHERE rid=?";
        std::vector<JobScDbValue> params;
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
        std::vector<JobScDbValue> params;
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
        if (CheckRequiredFields(row_data, required_fields))
        {
            JOBSC_LOG_ERROR("JobScDao::Update - missing required fields");
            break;
        }
        std::string sql = "UPDATE " + std::string(kTableName) + " SET " +
                          kFieldJobType + "=?, " +
                          kFieldDescription + "=?, " +
                          kFieldSettings + "=?, " +
                          kFieldAddressList + "=? " +
                          "WHERE rid=?";

        std::vector<JobScDbValue> params;
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

bool JobScDao::GetListByTypePage(const std::string& keyword, JobScType type, const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list)
{
    bool result{false};
    do
    {
        if (!m_db_ptr)
        {
            JOBSC_LOG_ERROR("JobScDao::GetListByTypePage - database not initialized");
            break;
        }
        std::string sql_main = "SELECT * FROM " + std::string(kTableName) + " WHERE " + kFieldJobType + "=?";
        std::vector<JobScDbValue> params;
        params.emplace_back(static_cast<int64_t>(type));
        result = m_db_ptr->QueryPageUniversal(sql_main, params, page_query, out_result, out_list);
        if (!result)
        {
            JOBSC_LOG_ERROR("JobScDao::GetListByTypePage - execute sql failed");
        }
    } while (false);
    return result;
}
