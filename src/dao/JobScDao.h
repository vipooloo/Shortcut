#ifndef JOBSCDAO_H
#define JOBSCDAO_H

#include "JobScTypes.h"
#include <memory>

class JobScDbAccess;
class JobScDbPageQuery;

class JobScDao
{
  public:
    class JobScTransGuard
    {
      public:
        explicit JobScTransGuard(JobScDao& dao);
        ~JobScTransGuard();
        void Commit();

      private:
        JobScTransGuard(const JobScTransGuard&) = delete;
        JobScTransGuard(JobScTransGuard&&) noexcept = delete;
        JobScTransGuard& operator=(const JobScTransGuard&) = delete;
        JobScTransGuard& operator=(JobScTransGuard&&) noexcept = delete;

      private:
        JobScDao& m_dao;
        bool m_committed;
    };

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
    uint64_t GetCountByType(uint64_t account_id, JobScType job_type);
    bool CheckRequiredFields(
        const JobScRow& row_data,
        const std::vector<std::string>& required_fields);

  private:
    void BeginTransaction();
    void CommitTransaction();
    void RollbackTransaction();
    bool GetAllPage(const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list);
    bool GetAllByKeywordPage(const std::string& keyword, const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list);
    bool GetListByTypePage(JobScType type, const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list);
    bool GetListByTypeAndKeywordPage(JobScType type, const std::string& keyword, const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list);

  private:
    std::shared_ptr<JobScDbAccess> m_db_ptr;
};

#endif  // JOBSCDAO_H