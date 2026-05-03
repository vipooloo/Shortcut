#ifndef JOBSCINITIALIZER_H
#define JOBSCINITIALIZER_H

#include "JobScDao.h"
#include <cstdint>

class JobScDbAccess;

class JobScInitializer
{
  public:
    JobScInitializer() = default;
    ~JobScInitializer() = default;

    void Init(JobScDao& dao);

    bool Upgrade(const std::shared_ptr<JobScDbAccess>& db_access_ptr, int64_t old_ver, int64_t new_ver);

  private:
};

#endif  // JOBSCINITIALIZER_H