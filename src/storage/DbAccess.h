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
    DbResult GetDbVersion(int32_t& out_version);
    DbResult SetDbVersion(int32_t version);

    DbResult ExecuteSql(const std::string& sql);
    DbResult ExecuteSql(const std::string& sql, const std::vector<std::string>& params);

    DbResult QuerySql(const std::string& sql, DbRows& out_rows);
    DbResult QuerySql(const std::string& sql, const std::vector<std::string>& params, DbRows& out_rows);

    DbResult BeginTransaction();
    DbResult Commit();
    DbResult Rollback();

    DbResult QueryPageUniversal(const std::string& sql_main,
                                const std::vector<std::string>& params,
                                const PageQuery& page_query,
                                PageResult& out_result,
                                DbRows& out_rows);

    void SetDbPath(const std::string& path);
    void SetUserVersion(int32_t version);

    using UpgradeCallback = std::function<DbResult(int32_t, int32_t)>;
    void SetUpgradeCallback(const UpgradeCallback& cb);

  private:
    static bool StrToInt32(const char* str, int32_t& out_val);
    static DbValue WrapColumnValue(const SQLite::Column& col);
    DbResult QueryTotalCount(const std::string& sql_main,
                             const std::vector<std::string>& params,
                             int32_t& total);
    DbResult QueryPageData(const std::string& sql_main,
                           const std::vector<std::string>& params,
                           const PageQuery& page_query,
                           int32_t total,
                           PageResult& out_result,
                           DbRows& out_rows);

  private:
    std::string m_db_path;
    int32_t m_user_version;
    std::unique_ptr<SQLite::Database> m_db_ptr;
    UpgradeCallback m_upgrade_callback;
};
}  // namespace shortcut

#endif