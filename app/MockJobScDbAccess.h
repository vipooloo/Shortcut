#ifndef MOCK_JOBSCDBACCESS_H
#define MOCK_JOBSCDBACCESS_H

#include "JobScDbAccess.h"
#include "gmock/gmock.h"

// Mock 类，继承自原始类 JobScDbAccess
class MockJobScDbAccess : public JobScDbAccess
{
  public:
    MockJobScDbAccess() = default;
    ~MockJobScDbAccess() override = default;

    // ===================== 所有虚函数全部 MOCK =====================
    MOCK_METHOD0(Init, bool());

    MOCK_METHOD1(ExecuteSql, bool(const std::string& sql));
    MOCK_METHOD2(ExecuteSql, bool(const std::string& sql, const std::vector<JobScValue>& params));

    MOCK_METHOD0(GetLastInsertRowId, int64_t());

    MOCK_METHOD2(QuerySql, bool(const std::string& sql, JobScRowList& out_rows));
    MOCK_METHOD3(QuerySql, bool(const std::string& sql, const std::vector<JobScValue>& params, JobScRowList& out_rows));

    MOCK_METHOD5(QueryPageUniversal,
                 bool(const std::string& sql_main,
                      const std::vector<JobScValue>& params,
                      const JobScDbPageQuery& page_query,
                      JobScPageResult& out_result,
                      JobScRowList& out_rows));
};

#endif  // MOCK_JOBSCDBACCESS_H