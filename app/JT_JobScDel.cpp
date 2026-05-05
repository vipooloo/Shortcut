

#include "MockJobScDbAccess.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#define private public
#define protected public
#include "JobScImpl.h"
#include "JobScMgr.h"
#include "JobScSqlDefines.h"

using namespace ::testing;

class JT_JobScDel : public ::testing::Test
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

TEST_F(JT_JobScDel, del)
{
    std::vector<int64_t> ids;
    {
        auto result = AddRecord();
        EXPECT_EQ(result.first, JobScResult::Success);
        ids.emplace_back(result.second.GetRid());
    }
    {
        auto result = AddRecord();
        EXPECT_EQ(result.first, JobScResult::Success);
        ids.emplace_back(result.second.GetRid());
    }
    EXPECT_EQ(ids.size(), 2);

    auto result = JobScMgr::Delete(ids);
    EXPECT_EQ(result, JobScResult::Success);
    JobScPageQuery page_query;
    JobScPageResult out_result;
    std::vector<JobScItem> out_items;
    auto query_result = JobScMgr::Query(page_query, out_result, out_items);
    EXPECT_EQ(query_result, JobScResult::Success);
    EXPECT_EQ(out_items.size(), 0);
}

TEST_F(JT_JobScDel, del_1)
{
    std::vector<int64_t> ids;
    auto result = JobScMgr::Delete(ids);
    EXPECT_EQ(result, JobScResult::InvalidParam);
    JobScPageQuery page_query;
    JobScPageResult out_result;
    std::vector<JobScItem> out_items;
    auto query_result = JobScMgr::Query(page_query, out_result, out_items);
    EXPECT_EQ(query_result, JobScResult::Success);
    EXPECT_EQ(out_items.size(), 0);
}

TEST_F(JT_JobScDel, del_2)
{
    std::pair<JobScResult, JobScItem> new_record = AddRecord();

    std::shared_ptr<JobScDbAccess> db_ptr = JobScImpl::GetInstance().m_dao.m_db_ptr;

    auto mock_db_ptr = std::make_shared<MockJobScDbAccess>();
    JobScImpl::GetInstance().m_dao.m_db_ptr = mock_db_ptr;
    EXPECT_CALL(*mock_db_ptr, ExecuteSql(_, _)).WillRepeatedly(Return(false));

    std::vector<int64_t> ids{1000, 10001};
    auto result = JobScMgr::Delete(ids);

    EXPECT_EQ(result, JobScResult::Failed);

    JobScImpl::GetInstance().m_dao.m_db_ptr = db_ptr;
}

TEST_F(JT_JobScDel, del_3)
{
    std::pair<JobScResult, JobScItem> new_record = AddRecord();

    std::shared_ptr<JobScDbAccess> db_ptr = JobScImpl::GetInstance().m_dao.m_db_ptr;

    auto mock_db_ptr = std::make_shared<MockJobScDbAccess>();
    JobScImpl::GetInstance().m_dao.m_db_ptr = mock_db_ptr;
    EXPECT_CALL(*mock_db_ptr, ExecuteSql(_, _)).WillRepeatedly(Return(false));

    std::vector<JobScType> types{JobScType::ScanToEmail};
    auto result = JobScMgr::DeleteByType(types);

    EXPECT_EQ(result, JobScResult::Failed);

    JobScImpl::GetInstance().m_dao.m_db_ptr = db_ptr;
}

TEST_F(JT_JobScDel, del_4)
{
    {
        std::vector<JobScType> types{JobScType::None};
        auto result = JobScMgr::DeleteByType(types);
        EXPECT_EQ(result, JobScResult::InvalidParam);
    }
    {
        std::vector<JobScType> types{JobScType::All};
        auto result = JobScMgr::DeleteByType(types);
        EXPECT_EQ(result, JobScResult::InvalidParam);
    }
    {
        std::vector<JobScType> types{JobScType::None, JobScType::All};
        auto result = JobScMgr::DeleteByType(types);
        EXPECT_EQ(result, JobScResult::InvalidParam);
    }
    {
        std::vector<JobScType> types{JobScType::None, JobScType::ScanToFTP};
        auto result = JobScMgr::DeleteByType(types);
        EXPECT_EQ(result, JobScResult::InvalidParam);
    }
    {
        std::vector<JobScType> types{JobScType::ScanToFTP, JobScType::All};
        auto result = JobScMgr::DeleteByType(types);
        EXPECT_EQ(result, JobScResult::InvalidParam);
    }
}