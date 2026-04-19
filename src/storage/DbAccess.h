#ifndef DBACCESS_H
#define DBACCESS_H

#include <SQLiteCpp/SQLiteCpp.h>

class DbAccess
{
public:
  static DbAccess& GetInstance()
  {
    static DbAccess instance;
    return instance;
  }

private:
  DbAccess();
  ~DbAccess() {}
  DbAccess(const DbAccess&) = delete;
  DbAccess& operator=(const DbAccess&) noexcept = delete;
  DbAccess(DbAccess&&) = delete;
  DbAccess& operator=(DbAccess&&) noexcept = delete;

private:
  SQLite::Database m_db;
};

#endif // DBACCESS_H