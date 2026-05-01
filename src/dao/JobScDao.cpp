#include "JobScDao.h"
#include "JobScLogger.h"

// 表名
constexpr const char* kTableName = "shortcut";

JobScDao::JobScDao(const std::shared_ptr<JobScDbAccess>& db_ptr)
  : m_db_ptr{db_ptr}
{
}

bool JobScDao::Insert(uint64_t account_id, const JobScRow& entity, int64_t& out_id)
{
    bool success = false;
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
        bool all_fields_exist = true;
        for (size_t i = 0; i < required_fields.size(); ++i)
        {
            const std::string& field = required_fields[i];
            if (entity.find(field) == entity.cend())
            {
                JOBSC_LOG_ERROR("JobScDao::Insert - missing required field: %s", field.c_str());
                all_fields_exist = false;
                break;
            }
        }
        if (!all_fields_exist)
        {
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
            params.push_back(entity.at(field));
        }
        success = m_db_ptr->ExecuteSql(sql, params);
        if (!success)
        {
            JOBSC_LOG_ERROR("JobScDao::Insert - execute sql failed");
            break;
        }
        out_id = m_db_ptr->GetLastInsertRowId();
    } while (false);
    return success;
}

// bool JobScDao::Update(const JobScRow& entity)
// {
//     if (!m_db_ptr)
//     {
//         return false;
//     }

//     auto it = entity.find("id");
//     if (it == entity.end())
//     {
//         JOB_SC_LOG_ERROR("Update: id not found");
//         return false;
//     }

//     std::string sql = "UPDATE " + kTableName + " SET name=?, type=?, content=?, updated_at=? WHERE id=?";
//     std::vector<JobScDbValue> params;

//     auto name_it = entity.find("name");
//     if (name_it != entity.end())
//     {
//         params.push_back(JobScDbValue(name_it->second.ToString()));
//     }
//     else
//     {
//         params.push_back(JobScDbValue(std::string("")));
//     }

//     auto type_it = entity.find("type");
//     if (type_it != entity.end())
//     {
//         params.push_back(JobScDbValue(static_cast<int32_t>(type_it->second)));
//     }
//     else
//     {
//         params.push_back(JobScDbValue(static_cast<int32_t>(0)));
//     }

//     auto content_it = entity.find("content");
//     if (content_it != entity.end())
//     {
//         params.push_back(content_it->second);
//     }
//     else
//     {
//         params.push_back(JobScDbValue(std::vector<uint8_t>{}));
//     }

//     int64_t now = 0;
//     params.push_back(JobScDbValue(now));
//     params.push_back(it->second);

//     return m_db_ptr->ExecuteSql(sql, params);
// }

// bool JobScDao::Delete(int64_t id)
// {
//     if (!m_db_ptr)
//     {
//         return false;
//     }

//     std::string sql = "DELETE FROM " + kTableName + " WHERE id=?";
//     std::vector<JobScDbValue> params;
//     params.push_back(JobScDbValue(id));

//     return m_db_ptr->ExecuteSql(sql, params);
// }

// bool JobScDao::GetById(int64_t id, JobScRow& out_entity)
// {
//     if (!m_db_ptr)
//     {
//         return false;
//     }

//     std::string sql = "SELECT * FROM " + kTableName + " WHERE id=?";
//     std::vector<JobScDbValue> params;
//     params.push_back(JobScDbValue(id));

//     JobScRowList rows;
//     if (!m_db_ptr->QuerySql(sql, params, rows))
//     {
//         return false;
//     }

//     if (!rows.empty())
//     {
//         out_entity = rows.front();
//         return true;
//     }

//     return false;
// }

// bool JobScDao::GetList(JobScRowList& out_list)
// {
//     if (!m_db_ptr)
//     {
//         return false;
//     }

//     std::string sql = "SELECT * FROM " + kTableName + " ORDER BY updated_at DESC";
//     return m_db_ptr->QuerySql(sql, out_list);
// }

// bool JobScDao::GetListPage(const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list)
// {
//     if (!m_db_ptr)
//     {
//         return false;
//     }

//     std::string sql = "SELECT * FROM " + kTableName;
//     return m_db_ptr->QueryPageUniversal(sql, {}, page_query, out_result, out_list);
// }

// bool JobScDao::GetListByType(ShortcutType type, JobScRowList& out_list)
// {
//     if (!m_db_ptr)
//     {
//         return false;
//     }

//     std::string sql = "SELECT * FROM " + kTableName + " WHERE type=? ORDER BY updated_at DESC";
//     std::vector<JobScDbValue> params;
//     params.push_back(JobScDbValue(static_cast<int32_t>(type)));

//     return m_db_ptr->QuerySql(sql, params, out_list);
// }

// bool JobScDao::GetListByTypePage(ShortcutType type, const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list)
// {
//     if (!m_db_ptr)
//     {
//         return false;
//     }

//     std::string sql = "SELECT * FROM " + kTableName + " WHERE type=?";
//     std::vector<JobScDbValue> params;
//     params.push_back(JobScDbValue(static_cast<int32_t>(type)));

//     return m_db_ptr->QueryPageUniversal(sql, params, page_query, out_result, out_list);
// }

// bool JobScDao::GetListByName(const std::string& name, JobScRowList& out_list)
// {
//     if (!m_db_ptr)
//     {
//         return false;
//     }

//     std::string sql = "SELECT * FROM " + kTableName + " WHERE name LIKE ? ORDER BY updated_at DESC";
//     std::vector<JobScDbValue> params;
//     params.push_back(JobScDbValue("%" + name + "%"));

//     return m_db_ptr->QuerySql(sql, params, out_list);
// }

// bool JobScDao::GetListByNamePage(const std::string& name, const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list)
// {
//     if (!m_db_ptr)
//     {
//         return false;
//     }

//     std::string sql = "SELECT * FROM " + kTableName + " WHERE name LIKE ?";
//     std::vector<JobScDbValue> params;
//     params.push_back(JobScDbValue("%" + name + "%"));

//     return m_db_ptr->QueryPageUniversal(sql, params, page_query, out_result, out_list);
// }