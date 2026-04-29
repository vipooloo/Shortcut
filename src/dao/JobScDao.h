#ifndef JOBSCDAO_H
#define JOBSCDAO_H

#include "JobScDbAccess.h"
#include <memory>

class ShortcutDao
{
  public:
    explicit ShortcutDao(const std::shared_ptr<JobScDbAccess>& db_ptr);
    ~ShortcutDao() = default;

  private:
    std::shared_ptr<JobScDbAccess> m_db_ptr;
};

#endif  // JOBSCDAO_H