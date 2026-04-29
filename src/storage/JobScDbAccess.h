#ifndef JOBSCDBACCESS_H
#define JOBSCDBACCESS_H

#include "JobScDbPageQuery.h"
#include "JobScDbValue.h"
#include "JobScSqlParam.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

using JobScEntity = std::map<std::string, JobScDbValue>;
using JobScEntityList = std::vector<JobScEntity>;

class JobScDbAccess
{
  public:
    JobScDbAccess();
    ~JobScDbAccess() = default;

    JobScDbAccess(const JobScDbAccess&) = delete;
    JobScDbAccess& operator=(const JobScDbAccess&) = delete;
    JobScDbAccess(JobScDbAccess&&) = delete;
    JobScDbAccess& operator=(JobScDbAccess&&) = delete;

    bool Init();

    bool ExecuteSql(const std::string& sql);
    bool ExecuteSql(const std::string& sql, const std::vector<JobScSqlParam>& params);

    bool QuerySql(const std::string& sql, JobScEntityList& out_rows);
    bool QuerySql(const std::string& sql, const std::vector<JobScSqlParam>& params, JobScEntityList& out_rows);

    void BeginTransaction();
    void CommitTransaction();
    void RollbackTransaction();

    bool QueryPageUniversal(const std::string& sql_main,
                            const std::vector<JobScSqlParam>& params,
                            const JobScDbPageQuery& page_query,
                            PageResult& out_result,
                            JobScEntityList& out_rows);

    void SetDbPath(const std::string& path);
    void SetUserVersion(int32_t version);

    using UpgradeCallback = std::function<bool(int64_t, int64_t)>;
    void SetUpgradeCallback(const UpgradeCallback& cb);

  private:
    bool GetDbVersion(int64_t& out_version);
    bool SetDbVersion(int64_t version);
    static JobScDbValue WrapColumnValue(const SQLite::Column& col);
    bool QueryTotalCount(const std::string& sql_main,
                         const std::vector<JobScSqlParam>& params,
                         uint32_t& total);
    bool QueryPageData(const std::string& sql_main,
                       const std::vector<JobScSqlParam>& params,
                       const JobScDbPageQuery& page_query,
                       uint32_t total,
                       PageResult& out_result,
                       JobScEntityList& out_rows);

  private:
    std::string m_db_path;
    int64_t m_user_version;
    std::unique_ptr<SQLite::Database> m_db_ptr;
    UpgradeCallback m_upgrade_callback;
};

#endif  // JOBSCDBACCESS_H