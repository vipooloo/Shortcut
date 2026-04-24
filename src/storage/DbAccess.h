#ifndef DB_ACCESS_H
#define DB_ACCESS_H

#include "DbResult.h"
#include "PageQuery.h"
#include "PageResult.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace shortcut {

class DbAccess
{
  public:
    DbAccess() = default;
    ~DbAccess() = default;

    DbAccess(const DbAccess&) = delete;
    DbAccess& operator=(const DbAccess&) = delete;
    DbAccess(DbAccess&&) = delete;
    DbAccess& operator=(DbAccess&&) = delete;

    DbResult Init();

    DbResult GetDBVersion(int32_t& out_version);
    DbResult SetDBVersion(int32_t version);

    DbResult ExecuteSql(const std::string& sql);
    DbResult ExecuteSql(const std::string& sql, const std::vector<std::string>& params);

    DbResult QuerySql(const std::string& sql, std::vector<std::map<std::string, std::string>>& out_rows);
    DbResult QuerySql(const std::string& sql, const std::vector<std::string>& params, std::vector<std::map<std::string, std::string>>& out_rows);

    DbResult BeginTransaction();
    DbResult Commit();
    DbResult Rollback();

    // 清爽接口：独立分页类，无状态，线程安全
    DbResult QueryPageUniversal(const std::string& sql_main,
                                const std::vector<std::string>& params,
                                const PageQuery& page_query,
                                PageResult& out_result);

    void SetDbPath(const std::string& path)
    {
        m_db_path = path;
    }
    void SetUserVersion(int32_t version)
    {
        m_user_version = version;
    }

    using UpgradeCallback = std::function<DbResult(int32_t, int32_t)>;
    void SetUpgradeCallback(UpgradeCallback cb)
    {
        m_upgrade_callback = std::move(cb);
    }

  private:
    std::string m_db_path{};
    int32_t m_user_version{0};
    std::unique_ptr<SQLite::Database> m_db_ptr{};
    UpgradeCallback m_upgrade_callback{};
};

}  // namespace shortcut

#endif