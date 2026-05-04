#ifndef JOBSCIMPL_H
#define JOBSCIMPL_H

#include "JobScDao.h"
#include "JobScInitializer.h"
#include "JobScItem.h"
#include "JobScResult.h"
#include <list>
#include <mutex>

class JobScImpl
{
  public:
    static JobScImpl& GetInstance();
    std::pair<JobScResult, int64_t> Add(uint64_t account_id, const JobScItem& item);
    JobScResult Delete(const std::vector<int64_t>& rids);
    JobScResult DeleteByType(const std::vector<JobScType>& types);
    JobScResult Update(int64_t rid, const JobScItem& item);

    void AddObserver(const JobScObserver& observer);

  private:
    JobScImpl();
    ~JobScImpl() = default;
    JobScImpl(const JobScImpl&) = delete;
    JobScImpl& operator=(const JobScImpl&) noexcept = delete;
    JobScImpl(JobScImpl&&) = delete;
    JobScImpl& operator=(JobScImpl&&) noexcept = delete;

  private:
    std::mutex m_service_mutex;
    JobScInitializer m_initializer;
    JobScDao m_dao;
    std::mutex m_observers_mutex;
    std::list<JobScObserver> m_observers;
};

#endif  // JOBSCIMPL_H