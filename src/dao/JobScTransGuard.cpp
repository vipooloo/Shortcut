#include "JobScDao.h"
#include "JobScDbAccess.h"

JobScDao::JobScTransGuard::JobScTransGuard(JobScDao& dao)
  : m_dao{dao}
  , m_committed{false}
{
    m_dao.BeginTransaction();
}

JobScDao::JobScTransGuard::~JobScTransGuard()
{
    m_dao.RollbackTransaction();
}

void JobScDao::JobScTransGuard::Commit()
{
    if (!m_committed)
    {
        m_dao.CommitTransaction();
        m_committed = true;
    }
}
