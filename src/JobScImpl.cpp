#include "JobScImpl.h"
#include "JobScLogger.h"

JobScImpl& JobScImpl::GetInstance()
{
    static JobScImpl instance;
    return instance;
}

JobScImpl::JobScImpl()
  : m_initializer{}
  , m_dao{}
{
    m_initializer.Init(m_dao);
}
