#include "DbAccess.h"

static constexpor const char* DB_FILE_PATH = "shortcut.db";

DbAccess::DbAccess()
  : m_db{ DB_FILE_PATH }
{
}