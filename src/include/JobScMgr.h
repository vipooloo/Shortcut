#ifndef JOBSCMGR_H
#define JOBSCMGR_H

#include "JobScItem.h"
#include "JobScPageQuery.h"
#include "JobScResult.h"
#include <cstdint>

class JobScMgr
{
  public:
    static std::pair<JobScResult, int64_t> Add(uint64_t account_id, const JobScItem& item);
    static JobScResult Delete(const std::vector<int64_t>& rids);
    static JobScResult DeleteByType(const std::vector<JobScType>& types);
    static JobScResult Update(int64_t rid, const JobScItem& item);
    static JobScResult GetListByTypePage(const JobScPageQuery& page_query, JobScPageResult& out_result, std::vector<JobScItem>& out_items);
    static void AddObserver(const JobScObserver& observer);
};

#endif  // JOBSCMGR_H