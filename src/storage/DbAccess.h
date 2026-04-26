#ifndef DB_ACCESS_H
#define DB_ACCESS_H

#include "DbResult.h"
#include "DbValue.h"
#include "PageQuery.h"
#include "PageResult.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace shortcut {
using DbRow = std::map<std::string, DbValue>;
using DbRows = std::vector<DbRow>;

class DbAccess
{
  public:
    DbAccess();
    ~DbAccess() = default;

    DbAccess(const DbAccess&) = delete;
    DbAccess& operator=(const DbAccess&) = delete;
    DbAccess(DbAccess&&) = delete;
    DbAccess& operator=(DbAccess&&) = delete;

    DbResult Init();
    DbResult GetDbVersion(int64_t& out_version);
    DbResult SetDbVersion(int64_t version);

    DbResult ExecuteSql(const std::string& sql)
    {
        return ExecuteSql(sql, std::vector<std::string>());
    }
    DbResult ExecuteSql(const std::string& sql, const std::vector<std::string>& params);

    DbResult QuerySql(const std::string& sql, DbRows& out_rows)
    {
        return QuerySql(sql, std::vector<std::string>(), out_rows);
    }
    DbResult QuerySql(const std::string& sql, const std::vector<std::string>& params, DbRows& out_rows);

    DbResult BeginTransaction()
    {
        return ExecuteSql(DB_SQL_BEGIN_TRANSACTION);
    }
    DbResult Commit()
    {
        return ExecuteSql(DB_SQL_COMMIT);
    }
    DbResult Rollback()
    {
        return ExecuteSql(DB_SQL_ROLLBACK);
    }

    DbResult QueryPageUniversal(const std::string& sql_main,
                                const std::vector<std::string>& params,
                                const PageQuery& page_query,
                                PageResult& out_result,
                                DbRows& out_rows);

    void SetDbPath(const std::string& path)
    {
        m_db_path = path;
    }
    void SetUserVersion(int32_t version)
    {
        m_user_version = version;
    }

    using UpgradeCallback = std::function<DbResult(int64_t, int64_t)>;
    void SetUpgradeCallback(const UpgradeCallback& cb)
    {
        m_upgrade_callback = cb;
    }

  private:
    static DbValue WrapColumnValue(const SQLite::Column& col);
    DbResult QueryTotalCount(const std::string& sql_main,
                             const std::vector<std::string>& params,
                             uint32_t& total);
    DbResult QueryPageData(const std::string& sql_main,
                           const std::vector<std::string>& params,
                           const PageQuery& page_query,
                           uint32_t total,
                           PageResult& out_result,
                           DbRows& out_rows);

  private:
    std::string m_db_path;
    int64_t m_user_version;
    std::unique_ptr<SQLite::Database> m_db_ptr;
    UpgradeCallback m_upgrade_callback;
};
}  // namespace shortcut

#endif