#include "JobScImpl.h"
#include "JobScLogger.h"
#include "JobScSqlDefines.h"

JobScImpl::JobScImpl()
  : m_service_mutex{}
  , m_initializer{}
  , m_dao{}
  , m_observers_mutex{}
  , m_observers{}
{
    m_initializer.Init(m_dao);
}

JobScImpl& JobScImpl::GetInstance()
{
    static JobScImpl instance;
    return instance;
}

void JobScImpl::AddObserver(const JobScObserver& observer)
{
    if (observer)
    {
        std::lock_guard<std::mutex> lock(m_observers_mutex);
        m_observers.emplace_back(observer);
    }
}

void JobScImpl::Notify(JobScEventType event)
{
    std::list<JobScObserver> observers;
    {
        std::lock_guard<std::mutex> lock(m_observers_mutex);
        observers = m_observers;
    }
    for (const JobScObserver& observer : observers)
    {
        if (observer)
        {
            observer(event);
        }
    }
}

std::pair<JobScResult, int64_t> JobScImpl::Add(uint64_t account_id, const JobScItem& item)
{
    JOBSC_LOG_INFO("JobScImpl::Add - account_id:%lu", account_id);
    std::pair<JobScResult, int64_t> result{JobScResult::Failed, INVALID_RID};

    JobScType job_type = item.GetJobType();
    std::lock_guard<std::mutex> lock(m_service_mutex);
    uint64_t count = m_dao.GetCountByType(account_id, job_type);
    if (count >= MAX_COUNT_PER_TYPE)
    {
        JOBSC_LOG_ERROR("JobScImpl::Add - max count reached for account_id:%lu job_type:%d", account_id, job_type);
        result.first = JobScResult::MaxCountReached;
    }
    else
    {
        JobScRow row_data{
            {kFieldJobType, JobScValue(static_cast<int64_t>(job_type))},
            {kFieldDescription, JobScValue(item.GetDescription())},
            {kFieldSettings, JobScValue(item.GetSettings())},
            {kFieldAddressList, JobScValue(item.GetAddressList())},
        };
        if (m_dao.Insert(account_id, row_data, result.second))
        {
            result.first = JobScResult::Success;
        }
    }
    return result;
}

JobScResult JobScImpl::Delete(const std::vector<int64_t>& rids)
{
    JOBSC_LOG_INFO("JobScImpl::Delete - rids size:%zu", rids.size());
    JobScResult result{JobScResult::Failed};

    std::lock_guard<std::mutex> lock(m_service_mutex);
    JobScDao::JobScTransGuard trans(m_dao);

    bool all_success{true};
    for (int64_t rid : rids)
    {
        if (!m_dao.Delete(rid))
        {
            JOBSC_LOG_ERROR("JobScImpl::Delete - failed to delete rid:%ld", rid);
            all_success = false;
            break;
        }
    }
    if (all_success)
    {
        trans.Commit();
        result = JobScResult::Success;
    }

    return result;
}

JobScResult JobScImpl::DeleteByType(const std::vector<JobScType>& types)
{
    JOBSC_LOG_INFO("JobScImpl::DeleteByType - types size:%zu", types.size());
    std::lock_guard<std::mutex> lock(m_service_mutex);
    JobScDao::JobScTransGuard trans(m_dao);

    bool all_success{true};
    for (JobScType type : types)
    {
        if (!m_dao.DeleteByType(type))
        {
            JOBSC_LOG_ERROR("JobScImpl::DeleteByType - failed to delete type:%d", type);
            all_success = false;
            break;
        }
    }
    if (all_success)
    {
        trans.Commit();
        return JobScResult::Success;
    }

    return JobScResult::Failed;
}

JobScResult JobScImpl::Update(int64_t rid, const JobScItem& item)
{
    JOBSC_LOG_INFO("JobScImpl::Update - rid:%ld", rid);
    JobScResult result{JobScResult::Failed};

    std::lock_guard<std::mutex> lock(m_service_mutex);
    JobScRow row_data{
        {kFieldJobType, JobScValue(static_cast<int64_t>(item.GetJobType()))},
        {kFieldDescription, JobScValue(item.GetDescription())},
        {kFieldSettings, JobScValue(item.GetSettings())},
        {kFieldAddressList, JobScValue(item.GetAddressList())},
    };

    if (m_dao.Update(rid, row_data))
    {
        JOBSC_LOG_INFO("JobScImpl::Update - success");
        result = JobScResult::Success;
    }
    else
    {
        JOBSC_LOG_ERROR("JobScImpl::Update - failed to update rid:%ld", rid);
    }
    return result;
}
