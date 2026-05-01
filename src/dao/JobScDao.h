#ifndef JOBSCDAO_H
#define JOBSCDAO_H

#include "JobScDbAccess.h"
#include "JobScType.h"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

constexpr const char* kFieldAccountId = "account_id";
constexpr const char* kFieldJobType = "job_type";
constexpr const char* kFieldDescription = "description";
constexpr const char* kFieldSettings = "settings";
constexpr const char* kFieldAddressList = "address_list";

class JobScDao
{
  public:
    explicit JobScDao(const std::shared_ptr<JobScDbAccess>& db_ptr);
    ~JobScDao() = default;

    bool Insert(uint64_t account_id, const JobScRow& row_data, int64_t& out_id);
    bool Delete(int64_t rid);
    bool DeleteByType(JobScType type);
    bool Update(int64_t rid, const JobScRow& row_data);

    bool GetListByTypePage(const std::string& keyword, JobScType type, const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list);

    // bool GetListByName(const std::string& name, JobScRowList& out_list);
    // bool GetListByNamePage(const std::string& name, const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list);
  private:
    bool CheckRequiredFields(
        const JobScRow& row_data,
        const std::vector<std::string>& required_fields);

  private:
    std::shared_ptr<JobScDbAccess> m_db_ptr;
};

#endif  // JOBSCDAO_H