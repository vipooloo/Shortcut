

#include "MockJobScDbAccess.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#define private public
#define protected public
#include "JobScImpl.h"
#include "JobScMgr.h"
#include "JobScSqlDefines.h"

using namespace ::testing;

class JT_JobScInit : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        std::vector<JobScType> types;
        for (JobScType type = static_cast<JobScType>(static_cast<int32_t>(JobScType::None) + 1); type < JobScType::All; type = static_cast<JobScType>(static_cast<int32_t>(type) + 1))
        {
            types.emplace_back(type);
        }
        auto result = JobScMgr::DeleteByType(types);
        EXPECT_EQ(result, JobScResult::Success);
    }
    void TearDown() override {}
    static void SetUpTestCase() {}
    static void TearDownTestCase()
    {
        std::vector<JobScType> types;
        for (JobScType type = static_cast<JobScType>(static_cast<int32_t>(JobScType::None) + 1); type < JobScType::All; type = static_cast<JobScType>(static_cast<int32_t>(type) + 1))
        {
            types.emplace_back(type);
        }
        auto result = JobScMgr::DeleteByType(types);
        EXPECT_EQ(result, JobScResult::Success);
    }
    std::pair<JobScResult, JobScItem> AddRecord()
    {
        auto observer = [](JobScEventType event) {
            EXPECT_EQ(event, JobScEventType::Added);
        };
        JobScMgr::AddObserver(observer);
        std::vector<uint8_t> job_setting{0x01, 0x02};
        std::vector<uint8_t> address_list{0x11, 0x22};
        std::pair<JobScResult, JobScItem> result = JobScMgr::Add(1234, JobScType::ScanToEmail, "test", job_setting, address_list);

        JobScMgr::RemoveObserver(observer);
        return result;
    }
};

TEST_F(JT_JobScInit, init)
{
    std::shared_ptr<JobScDbAccess> db_ptr = JobScImpl::GetInstance().m_dao.m_db_ptr;

    auto mock_db_ptr = std::make_shared<MockJobScDbAccess>();
    JobScImpl::GetInstance().m_dao.m_db_ptr = mock_db_ptr;
    EXPECT_CALL(*mock_db_ptr, Init()).WillRepeatedly(Return(false));

    JobScInitializer initializer;
    initializer.Init(JobScImpl::GetInstance().m_dao);

    JobScImpl::GetInstance().m_dao.m_db_ptr = db_ptr;
}
