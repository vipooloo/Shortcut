#include "JobScMgr.h"
#include "JobScImpl.h"

std::pair<JobScResult, int64_t> JobScMgr::Add(uint64_t account_id, const JobScItem& item)
{
    return JobScImpl::GetInstance().Add(account_id, item);
}

JobScResult JobScMgr::Delete(int64_t rid)
{}

JobScResult JobScMgr::DeleteByType(JobScType type)
{}