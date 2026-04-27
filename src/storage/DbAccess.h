#ifndef DB_ACCESS_H
#define DB_ACCESS_H

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

    bool Init();
    bool GetDbVersion(int64_t& out_version);
    bool SetDbVersion(int64_t version);

    bool ExecuteSql(const std::string& sql)
    {
        return ExecuteSql(sql, std::vector<std::string>());
    }
    bool ExecuteSql(const std::string& sql, const std::vector<std::string>& params);

    bool QuerySql(const std::string& sql, DbRows& out_rows)
    {
        return QuerySql(sql, std::vector<std::string>(), out_rows);
    }
    bool QuerySql(const std::string& sql, const std::vector<std::string>& params, DbRows& out_rows);

    bool BeginTransaction()
    {
        return ExecuteSql(DB_SQL_BEGIN_TRANSACTION);
    }
    bool Commit()
    {
        return ExecuteSql(DB_SQL_COMMIT);
    }
    bool Rollback()
    {
        return ExecuteSql(DB_SQL_ROLLBACK);
    }

    bool QueryPageUniversal(const std::string& sql_main,
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

    using UpgradeCallback = std::function<bool(int64_t, int64_t)>;
    void SetUpgradeCallback(const UpgradeCallback& cb)
    {
        m_upgrade_callback = cb;
    }

  private:
    static DbValue WrapColumnValue(const SQLite::Column& col);
    bool QueryTotalCount(const std::string& sql_main,
                         const std::vector<std::string>& params,
                         uint32_t& total);
    bool QueryPageData(const std::string& sql_main,
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