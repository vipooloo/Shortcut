#ifndef JOBSCDAO_H
#define JOBSCDAO_H

#include "JobScDbAccess.h"
#include "ShortcutType.h"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class JobScDao
{
  public:
    explicit JobScDao(const std::shared_ptr<JobScDbAccess>& db_ptr);
    ~JobScDao() = default;

    bool Insert(int64_t& out_id, const JobScEntity& entity);
    // bool Update(const JobScEntity& entity);
    // bool Delete(int64_t id);

    // bool GetById(int64_t id, JobScEntity& out_entity);
    // bool GetList(JobScEntityList& out_list);
    // bool GetListPage(const JobScDbPageQuery& page_query, PageResult& out_result, JobScEntityList& out_list);
    // bool GetListByType(ShortcutType type, JobScEntityList& out_list);
    // bool GetListByTypePage(ShortcutType type, const JobScDbPageQuery& page_query, PageResult& out_result, JobScEntityList& out_list);

    // bool GetListByName(const std::string& name, JobScEntityList& out_list);
    // bool GetListByNamePage(const std::string& name, const JobScDbPageQuery& page_query, PageResult& out_result, JobScEntityList& out_list);

  private:
    std::shared_ptr<JobScDbAccess> m_db_ptr;
};

#endif  // JOBSCDAO_H