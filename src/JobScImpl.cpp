#include "JobScImpl.h"
#include "JobScDbPageQuery.h"
#include "JobScLogger.h"
#include "JobScSqlDefines.h"
#include <algorithm>

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

void JobScImpl::RemoveObserver(const JobScObserver& observer)
{
    using FuncPtr = void (*)(JobScEventType event);
    if (observer)
    {
        std::lock_guard<std::mutex> lock(m_observers_mutex);
        m_observers.erase(std::remove_if(m_observers.begin(), m_observers.end(), [&observer](const JobScObserver& ob) {
                              return ob.target<FuncPtr>() == observer.target<FuncPtr>();
                          }),
                          m_observers.end());
    }
}

void JobScImpl::Notify(JobScEventType event)
{
    JOBSC_LOG_INFO("JobScImpl::Notify - event:%d start", static_cast<int32_t>(event));
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
    JOBSC_LOG_INFO("JobScImpl::Notify - event:%d end", static_cast<int32_t>(event));
}

std::pair<JobScResult, JobScItem> JobScImpl::Add(uint64_t account_id,
                                                 JobScType type,
                                                 const std::string& description,
                                                 const std::vector<uint8_t>& job_settings,
                                                 const std::vector<uint8_t>& addr_info)
{
    JOBSC_LOG_INFO("JobScImpl::Add - account_id:%lu type:%d", account_id, type);
    std::pair<JobScResult, JobScItem> result{JobScResult::Failed, JobScItem{}};
    // 参数检查
    if ((type > JobScType::None) && (type < JobScType::All) && (!description.empty()))
    {
        JobScItem item;
        item.SetAccountId(account_id);
        item.SetJobType(type);
        item.SetDescription(description);
        item.SetSettings(job_settings);
        item.SetAddressList(addr_info);
        std::pair<JobScResult, int64_t> add_res = Add(account_id, item);
        result.first = add_res.first;
        if (JobScResult::Success == result.first)
        {
            item.SetRid(add_res.second);
            result.second = std::move(item);
        }
    }
    else
    {
        JOBSC_LOG_ERROR("JobScImpl::Add - invalid param");
        result.first = JobScResult::InvalidParam;
    }
    return result;
}

std::pair<JobScResult, int64_t> JobScImpl::Add(uint64_t account_id, const JobScItem& item)
{
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
            JOBSC_LOG_INFO("JobScImpl::Add - success account_id:%lu job_type:%d rid:%ld", account_id, job_type, result.second);
            Notify(JobScEventType::Added);
        }
        else
        {
            JOBSC_LOG_ERROR("JobScImpl::Add - failed to insert account_id:%lu job_type:%d", account_id, job_type);
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
        JOBSC_LOG_INFO("JobScImpl::Delete - success");
        Notify(JobScEventType::Deleted);
    }

    return result;
}

JobScResult JobScImpl::DeleteByType(const std::vector<JobScType>& types)
{
    JobScResult result{JobScResult::Failed};
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
        result = JobScResult::Success;
        JOBSC_LOG_INFO("JobScImpl::Delete - success");
        Notify(JobScEventType::Deleted);
    }

    return result;
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
        result = JobScResult::Success;
        JOBSC_LOG_INFO("JobScImpl::Delete - success");
        Notify(JobScEventType::Deleted);
    }
    else
    {
        JOBSC_LOG_ERROR("JobScImpl::Update - failed to update rid:%ld", rid);
    }
    return result;
}

JobScResult JobScImpl::GetListByTypePage(
    const JobScPageQuery& page_query,
    JobScPageResult& out_result,
    std::vector<JobScItem>& out_items)
{
    JobScResult ret{JobScResult::Failed};
    JobScOrderType order_type = page_query.GetOrderType();
    uint32_t page_index = page_query.GetPageIndex();
    uint32_t page_size = page_query.GetPageSize();
    JOBSC_LOG_INFO("JobScImpl::GetListByTypePage - order_type:%d page_index:%u page_size:%u", static_cast<int32_t>(order_type), page_index, page_size);

    JobScRowList row_list;
    bool result{false};
    {
        std::lock_guard<std::mutex> lock(m_service_mutex);
        result = m_dao.GetListByTypePage(
            page_query.GetKeyword(),
            page_query.GetType(),
            JobScDbPageQuery{kFieldRid, page_query.GetOrderType(), page_query.GetPageIndex(), page_query.GetPageSize()},
            out_result,
            row_list);
    }
    if (result)
    {
        JOBSC_LOG_INFO("JobScImpl::GetListByTypePage - success total_count:%u", out_result.total_count);
        out_items = CovertrToItems(row_list);
        ret = JobScResult::Success;
    }
    else
    {
        JOBSC_LOG_ERROR("JobScImpl::GetListByTypePage - failed");
    }
    return ret;
}

std::vector<JobScItem> JobScImpl::CovertrToItems(const JobScRowList& rows)
{
    std::vector<JobScItem> items;
    const std::vector<std::string> required_fields = {
        kFieldRid,
        kFieldJobType,
        kFieldDescription,
        kFieldSettings,
        kFieldAddressList};
    bool result = std::all_of(rows.cbegin(), rows.cend(), [this, &required_fields](const JobScRow& row) {
        return m_dao.CheckRequiredFields(row, required_fields);
    });
    if (result)
    {
        for (const JobScRow& row : rows)
        {
            JobScItem item;
            int64_t rid = row.at(kFieldRid);
            item.SetRid(rid);
            int32_t job_type = row.at(kFieldJobType);
            JobScType jobsc_type = static_cast<JobScType>(job_type);
            item.SetJobType(jobsc_type);
            const std::string& description = row.at(kFieldDescription);
            item.SetDescription(description);
            const std::vector<uint8_t>& settings = row.at(kFieldSettings);
            item.SetSettings(settings);
            const std::vector<uint8_t>& addr_info = row.at(kFieldAddressList);
            item.SetAddressList(addr_info);
            items.emplace_back(std::move(item));
        }
    }
    return items;
}