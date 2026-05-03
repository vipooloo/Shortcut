#ifndef JOBSCIMPL_H
#define JOBSCIMPL_H

#include "JobScDao.h"
#include "JobScInitializer.h"

class JobScImpl
{
  public:
    static JobScImpl& GetInstance();

  private:
    JobScImpl();
    ~JobScImpl() = default;
    JobScImpl(const JobScImpl&) = delete;
    JobScImpl& operator=(const JobScImpl&) noexcept = delete;
    JobScImpl(JobScImpl&&) = delete;
    JobScImpl& operator=(JobScImpl&&) noexcept = delete;

  private:
    JobScInitializer m_initializer;
    JobScDao m_dao;
};

#endif  // JOBSCIMPL_H