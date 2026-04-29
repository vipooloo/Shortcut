#include "JobScDao.h"
#include "JobScLogger.h"

ShortcutDao::ShortcutDao(const std::shared_ptr<JobScDbAccess>& db_ptr)
  : m_db_ptr{db_ptr}
{
}
