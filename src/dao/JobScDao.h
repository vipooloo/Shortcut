#ifndef JOBSCDAO_H
#define JOBSCDAO_H

#include "JobScTypes.h"
#include <memory>

class JobScDbAccess;
class JobScDbPageQuery;

class JobScDao
{
  public:
    JobScDao();
    explicit JobScDao(const std::shared_ptr<JobScDbAccess>& db_ptr);
    ~JobScDao() = default;
    void SetDbAccess(const std::shared_ptr<JobScDbAccess>& db_ptr)
    {
        m_db_ptr = db_ptr;
    }
    bool Insert(uint64_t account_id, const JobScRow& row_data, int64_t& out_id);
    bool Delete(int64_t rid);
    bool DeleteByType(JobScType type);
    bool Update(int64_t rid, const JobScRow& row_data);

    bool GetListByTypePage(
        const std::string& keyword,
        JobScType type,
        const JobScDbPageQuery& page_query,
        JobScPageResult& out_result,
        JobScRowList& out_list);

  private:
    bool CheckRequiredFields(
        const JobScRow& row_data,
        const std::vector<std::string>& required_fields);
    bool GetAllPage(const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list);
    bool GetAllByKeywordPage(const std::string& keyword, const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list);
    bool GetListByTypePage(JobScType type, const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list);
    bool GetListByTypeAndKeywordPage(JobScType type, const std::string& keyword, const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list);

  private:
    std::shared_ptr<JobScDbAccess> m_db_ptr;
};

#endif  // JOBSCDAO_H