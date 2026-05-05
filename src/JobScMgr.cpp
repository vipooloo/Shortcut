#include "JobScMgr.h"
#include "JobScImpl.h"

std::pair<JobScResult, JobScItem> JobScMgr::Add(uint64_t account_id,
                                                JobScType type,
                                                const std::string& description,
                                                const std::vector<uint8_t>& job_settings,
                                                const std::vector<uint8_t>& addr_info)
{
    return JobScImpl::GetInstance().Add(account_id, type, description, job_settings, addr_info);
}

JobScResult JobScMgr::Delete(const std::vector<int64_t>& rids)
{
    return JobScImpl::GetInstance().Delete(rids);
}

JobScResult JobScMgr::DeleteByType(const std::vector<JobScType>& types)
{
    return JobScImpl::GetInstance().DeleteByType(types);
}

JobScResult JobScMgr::Update(const JobScItem& item)
{
    return JobScImpl::GetInstance().Update(item);
}

JobScResult JobScMgr::Query(
    const JobScPageQuery& page_query,
    JobScPageResult& out_result,
    std::vector<JobScItem>& out_items)
{
    return JobScImpl::GetInstance().Query(page_query, out_result, out_items);
}

void JobScMgr::AddObserver(const JobScObserver& observer)
{
    JobScImpl::GetInstance().AddObserver(observer);
}

void JobScMgr::RemoveObserver(const JobScObserver& observer)
{
    JobScImpl::GetInstance().RemoveObserver(observer);
}