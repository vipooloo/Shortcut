#ifndef JOBSCDAO_H
#define JOBSCDAO_H

#include "JobScDbAccess.h"
#include "ShortcutType.h"
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

    bool Insert(uint64_t account_id, const JobScRow& entity, int64_t& out_id);
    // bool Update(const JobScRow& entity);
    // bool Delete(int64_t id);

    // bool GetById(int64_t id, JobScRow& out_entity);
    // bool GetList(JobScRowList& out_list);
    // bool GetListPage(const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list);
    // bool GetListByType(ShortcutType type, JobScRowList& out_list);
    // bool GetListByTypePage(ShortcutType type, const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list);

    // bool GetListByName(const std::string& name, JobScRowList& out_list);
    // bool GetListByNamePage(const std::string& name, const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list);

  private:
    std::shared_ptr<JobScDbAccess> m_db_ptr;
};

#endif  // JOBSCDAO_H