#ifndef JOBSCDBACCESS_H
#define JOBSCDBACCESS_H

#include "JobScDbPageQuery.h"
#include "JobScDbValue.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

using JobScRow = std::map<std::string, JobScDbValue>;
using JobScRowList = std::vector<JobScRow>;
using UpgradeCallback = std::function<bool(int64_t, int64_t)>;

class JobScDbAccess
{
  public:
    JobScDbAccess();
    ~JobScDbAccess() = default;

    bool Init();
    void SetDbPath(const std::string& path)
    {
        m_db_path = path;
    }
    void SetUserVersion(int32_t version)
    {
        m_user_version = version;
    }
    void SetUpgradeCallback(const UpgradeCallback& cb)
    {
        m_upgrade_callback = cb;
    }

    bool ExecuteSql(const std::string& sql);
    bool ExecuteSql(const std::string& sql, const std::vector<JobScDbValue>& params);
    int64_t GetLastInsertRowId();

    bool QuerySql(const std::string& sql, JobScRowList& out_rows);
    bool QuerySql(const std::string& sql, const std::vector<JobScDbValue>& params, JobScRowList& out_rows);

    void BeginTransaction();
    void CommitTransaction();
    void RollbackTransaction();

    bool QueryPageUniversal(const std::string& sql_main,
                            const std::vector<JobScDbValue>& params,
                            const JobScDbPageQuery& page_query,
                            JobScPageResult& out_result,
                            JobScRowList& out_rows);

  private:
    JobScDbAccess(const JobScDbAccess&) = delete;
    JobScDbAccess& operator=(const JobScDbAccess&) = delete;
    JobScDbAccess(JobScDbAccess&&) = delete;
    JobScDbAccess& operator=(JobScDbAccess&&) = delete;
    bool GetDbVersion(int64_t& out_version);
    bool SetDbVersion(int64_t version);
    static JobScDbValue WrapColumnValue(const SQLite::Column& col);
    bool QueryTotalCount(const std::string& sql_main,
                         const std::vector<JobScDbValue>& params,
                         uint32_t& total);
    bool QueryPageData(const std::string& sql_main,
                       const std::vector<JobScDbValue>& params,
                       const JobScDbPageQuery& page_query,
                       uint32_t total,
                       JobScPageResult& out_result,
                       JobScRowList& out_rows);

  private:
    std::string m_db_path;
    int64_t m_user_version;
    std::unique_ptr<SQLite::Database> m_db_ptr;
    UpgradeCallback m_upgrade_callback;
};

#endif  // JOBSCDBACCESS_H