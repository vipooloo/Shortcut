#ifndef DB_ACCESS_H
#define DB_ACCESS_H

#include "DbValue.h"
#include "PageQuery.h"
#include "PageResult.h"
#include "SqlParam.h"
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

    bool ExecuteSql(const std::string& sql);
    bool ExecuteSql(const std::string& sql, const std::vector<SqlParam>& params);

    bool QuerySql(const std::string& sql, DbRows& out_rows);
    bool QuerySql(const std::string& sql, const std::vector<SqlParam>& params, DbRows& out_rows);

    void BeginTransaction();
    void CommitTransaction();
    void RollbackTransaction();

    bool QueryPageUniversal(const std::string& sql_main,
                            const std::vector<SqlParam>& params,
                            const PageQuery& page_query,
                            PageResult& out_result,
                            DbRows& out_rows);

    void SetDbPath(const std::string& path);
    void SetUserVersion(int32_t version);

    using UpgradeCallback = std::function<bool(int64_t, int64_t)>;
    void SetUpgradeCallback(const UpgradeCallback& cb);

  private:
    bool GetDbVersion(int64_t& out_version);
    bool SetDbVersion(int64_t version);
    static DbValue WrapColumnValue(const SQLite::Column& col);
    bool QueryTotalCount(const std::string& sql_main,
                         const std::vector<SqlParam>& params,
                         uint32_t& total);
    bool QueryPageData(const std::string& sql_main,
                       const std::vector<SqlParam>& params,
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