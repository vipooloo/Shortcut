#ifndef JOBSCMGR_H
#define JOBSCMGR_H

#include "JobScItem.h"
#include "JobScResult.h"
#include <cstdint>

class JobScMgr
{
  public:
    static std::pair<JobScResult, int64_t> Add(uint64_t account_id, const JobScItem& item);
    static JobScResult Delete(int64_t rid);
    static JobScResult DeleteByType(JobScType type);
};

#endif  // JOBSCMGR_H