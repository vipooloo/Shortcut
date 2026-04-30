#include "JobScDao.h"
#include "JobScLogger.h"

static const std::string kTableName = "shortcut";

JobScDao::JobScDao(const std::shared_ptr<JobScDbAccess>& db_ptr)
  : m_db_ptr{db_ptr}
{
}

bool JobScDao::Insert(int64_t& out_id, const JobScEntity& entity)
{
    // if (!m_db_ptr)
    // {
    //     return false;
    // }

    // std::string sql = "INSERT INTO " + kTableName + " (name, type, content, created_at, updated_at) VALUES (?, ?, ?, ?, ?)";
    // std::vector<JobScDbValue> params;

    // auto it = entity.find("name");
    // if (it != entity.end())
    // {
    //     params.push_back(JobScDbValue(it->second.ToString()));
    // }
    // else
    // {
    //     params.push_back(JobScDbValue(std::string("")));
    // }

    // it = entity.find("type");
    // if (it != entity.end())
    // {
    //     params.push_back(JobScDbValue(static_cast<int32_t>(it->second)));
    // }
    // else
    // {
    //     params.push_back(JobScDbValue(static_cast<int32_t>(0)));
    // }

    // it = entity.find("content");
    // if (it != entity.end())
    // {
    //     params.push_back(it->second);
    // }
    // else
    // {
    //     params.push_back(JobScDbValue(std::vector<uint8_t>{}));
    // }

    // int64_t now = 0;
    // params.push_back(JobScDbValue(now));
    // params.push_back(JobScDbValue(now));

    // if (!m_db_ptr->ExecuteSql(sql, params))
    // {
    //     JOB_SC_LOG_ERROR("Insert failed");
    //     return false;
    // }

    // out_id = m_db_ptr->GetLastInsertId();
    return true;
}

// bool JobScDao::Update(const JobScEntity& entity)
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

// bool JobScDao::GetById(int64_t id, JobScEntity& out_entity)
// {
//     if (!m_db_ptr)
//     {
//         return false;
//     }

//     std::string sql = "SELECT * FROM " + kTableName + " WHERE id=?";
//     std::vector<JobScDbValue> params;
//     params.push_back(JobScDbValue(id));

//     JobScEntityList rows;
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

// bool JobScDao::GetList(JobScEntityList& out_list)
// {
//     if (!m_db_ptr)
//     {
//         return false;
//     }

//     std::string sql = "SELECT * FROM " + kTableName + " ORDER BY updated_at DESC";
//     return m_db_ptr->QuerySql(sql, out_list);
// }

// bool JobScDao::GetListPage(const JobScDbPageQuery& page_query, PageResult& out_result, JobScEntityList& out_list)
// {
//     if (!m_db_ptr)
//     {
//         return false;
//     }

//     std::string sql = "SELECT * FROM " + kTableName;
//     return m_db_ptr->QueryPageUniversal(sql, {}, page_query, out_result, out_list);
// }

// bool JobScDao::GetListByType(ShortcutType type, JobScEntityList& out_list)
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

// bool JobScDao::GetListByTypePage(ShortcutType type, const JobScDbPageQuery& page_query, PageResult& out_result, JobScEntityList& out_list)
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

// bool JobScDao::GetListByName(const std::string& name, JobScEntityList& out_list)
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

// bool JobScDao::GetListByNamePage(const std::string& name, const JobScDbPageQuery& page_query, PageResult& out_result, JobScEntityList& out_list)
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