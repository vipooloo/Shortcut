

#include "MockJobScDbAccess.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#define private public
#define protected public
#include "JobScImpl.h"
#include "JobScMgr.h"
#include "JobScSqlDefines.h"
#include "JobScValue.h"

using namespace ::testing;

class JT_JobScDao : public ::testing::Test
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

TEST_F(JT_JobScDao, Insert_1)
{
    JobScDao dao(nullptr);
    uint64_t account_id{0u};
    JobScRow row_data;
    int64_t out_id{0};

    auto result = dao.Insert(123, row_data, out_id);
    EXPECT_FALSE(result);
}

TEST_F(JT_JobScDao, Insert_2)
{
    JobScDao dao(std::make_shared<JobScDbAccess>());
    uint64_t account_id{0u};
    JobScRow row_data;
    int64_t out_id{0};

    auto result = dao.Insert(123, row_data, out_id);
    EXPECT_FALSE(result);
}

TEST_F(JT_JobScDao, Delete_1)
{
    JobScDao dao(nullptr);
    auto result = dao.Delete(123);
    EXPECT_FALSE(result);
}

TEST_F(JT_JobScDao, DeleteByType_1)
{
    JobScDao dao(nullptr);
    auto result = dao.DeleteByType(JobScType::ScanToEmail);
    EXPECT_FALSE(result);
}

TEST_F(JT_JobScDao, Update_1)
{
    JobScDao dao(nullptr);
    JobScRow row_data;
    auto result = dao.Update(123, row_data);
    EXPECT_FALSE(result);
}

TEST_F(JT_JobScDao, Update_2)
{
    JobScDao dao(std::make_shared<JobScDbAccess>());
    JobScRow row_data;
    auto result = dao.Update(123, row_data);
    EXPECT_FALSE(result);
}

TEST_F(JT_JobScDao, Update_3)
{
    auto mock = std::make_shared<MockJobScDbAccess>();
    JobScDao dao(mock);
    EXPECT_CALL(*mock, ExecuteSql(_, _, _)).WillRepeatedly(Return(false));
    std::vector<uint8_t> job_setting{0x01, 0x02};
    std::vector<uint8_t> address_list{0x11, 0x22};
    JobScRow row_data{
        {kFieldJobType, JobScValue(static_cast<int64_t>(JobScType::ScanToEmail))},
        {kFieldDescription, JobScValue("test")},
        {kFieldSettings, JobScValue(job_setting)},
        {kFieldAddressList, JobScValue(address_list)},
    };
    auto result = dao.Update(123, row_data);
    EXPECT_FALSE(result);
}

TEST_F(JT_JobScDao, Query_1)
{
    auto mock = std::make_shared<MockJobScDbAccess>();
    JobScDao dao(mock);
    std::string keyword{"123"};
    JobScType type{JobScType::All};
    JobScDbPageQuery page_query;
    JobScPageResult out_result;
    JobScRowList out_list;
    EXPECT_CALL(*mock, QueryPageUniversal(_, _, _, _, _)).WillRepeatedly(Return(false));
    bool ret = dao.Query(keyword, type, page_query, out_result, out_list);
    EXPECT_FALSE(ret);
}

TEST_F(JT_JobScDao, Query_2)
{
    JobScDao dao(nullptr);
    std::string keyword;
    JobScType type{JobScType::None};
    JobScDbPageQuery page_query;
    JobScPageResult out_result;
    JobScRowList out_list;
    bool ret = dao.Query(keyword, type, page_query, out_result, out_list);
    EXPECT_FALSE(ret);
}

TEST_F(JT_JobScDao, GetCountByType)
{
    JobScDao dao(nullptr);
    bool ret = dao.GetCountByType(12, JobScType::ScanToEmail);
    EXPECT_FALSE(ret);
}