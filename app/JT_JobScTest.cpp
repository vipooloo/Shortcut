/**
 * @file JT_JobScTest.cpp
 * @brief 作业调度系统集成测试
 * @details 从集成测试角度测试JobScMgr的完整业务流程
 */

#include "../src/JobScValue.h"
#include "JobScDefines.h"
#include "JobScItem.h"
#include "JobScMgr.h"
#include "JobScPageQuery.h"
#include "JobScResult.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>
#include <vector>

class JT_JobScItemTest : public ::testing::Test
{
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(JT_JobScItemTest, CreateDefaultItem)
{
    JobScItem item;
    EXPECT_EQ(item.GetRid(), -1);
    EXPECT_EQ(item.GetAccountId(), 0);
    EXPECT_EQ(item.GetJobType(), JobScType::None);
    EXPECT_EQ(item.GetDescription(), "");
    EXPECT_TRUE(item.GetSettings().empty());
    EXPECT_TRUE(item.GetAddressList().empty());
}

TEST_F(JT_JobScItemTest, SetAndGetItemProperties)
{
    JobScItem item;
    item.SetRid(100);
    item.SetAccountId(123456789);
    item.SetJobType(JobScType::ScanToEmail);
    item.SetDescription("Test Job");

    std::vector<uint8_t> settings = {1, 2, 3, 4};
    item.SetSettings(settings);

    std::vector<uint8_t> address = {5, 6, 7, 8};
    item.SetAddressList(address);

    EXPECT_EQ(item.GetRid(), 100);
    EXPECT_EQ(item.GetAccountId(), 123456789);
    EXPECT_EQ(item.GetJobType(), JobScType::ScanToEmail);
    EXPECT_EQ(item.GetDescription(), "Test Job");
    EXPECT_EQ(item.GetSettings(), settings);
    EXPECT_EQ(item.GetAddressList(), address);
}

TEST_F(JT_JobScItemTest, JobTypeEnumValues)
{
    EXPECT_EQ(static_cast<int>(JobScType::None), 0);
    EXPECT_EQ(static_cast<int>(JobScType::ScanToEmail), 1);
    EXPECT_EQ(static_cast<int>(JobScType::ScanToFTP), 2);
    EXPECT_EQ(static_cast<int>(JobScType::ScanToSMB), 3);
    EXPECT_EQ(static_cast<int>(JobScType::ScanToUSB), 4);
    EXPECT_EQ(static_cast<int>(JobScType::All), 5);
}

class JT_JobScPageQueryTest : public ::testing::Test
{
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(JT_JobScPageQueryTest, DefaultPageQuery)
{
    JobScPageQuery query;
    EXPECT_EQ(query.GetKeyword(), "");
    EXPECT_EQ(query.GetType(), JobScType::None);
    EXPECT_EQ(query.GetOrderType(), JobScOrderType::ASC);
    EXPECT_EQ(query.GetPageIndex(), JOBSC_PAGE_DEFAULT_INDEX);
    EXPECT_EQ(query.GetPageSize(), JOBSC_PAGE_DEFAULT_SIZE);
}

TEST_F(JT_JobScPageQueryTest, SetPageQueryParameters)
{
    JobScPageQuery query;
    query.SetKeyword("test");
    query.SetType(JobScType::ScanToFTP);
    query.SetOrderType(JobScOrderType::DESC);
    query.SetPageIndex(2);
    query.SetPageSize(20);

    EXPECT_EQ(query.GetKeyword(), "test");
    EXPECT_EQ(query.GetType(), JobScType::ScanToFTP);
    EXPECT_EQ(query.GetOrderType(), JobScOrderType::DESC);
    EXPECT_EQ(query.GetPageIndex(), 2);
    EXPECT_EQ(query.GetPageSize(), 20);
}

class JT_JobScValueTest : public ::testing::Test
{
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(JT_JobScValueTest, CreateNullValue)
{
    JobScValue value;
    EXPECT_TRUE(value.IsNull());
    EXPECT_EQ(value.GetType(), JobScValType::Null);
}

TEST_F(JT_JobScValueTest, CreateIntValue)
{
    JobScValue value(static_cast<int64_t>(100));
    EXPECT_FALSE(value.IsNull());
    EXPECT_EQ(value.GetType(), JobScValType::Int);

    bool ok = false;
    int64_t intVal = value.ToInt64(ok);
    EXPECT_TRUE(ok);
    EXPECT_EQ(intVal, 100);
}

TEST_F(JT_JobScValueTest, CreateUintValue)
{
    JobScValue value(static_cast<uint64_t>(200));
    EXPECT_FALSE(value.IsNull());
    EXPECT_EQ(value.GetType(), JobScValType::Int);

    bool ok = false;
    uint64_t uintVal = value.ToUint64(ok);
    EXPECT_TRUE(ok);
    EXPECT_EQ(uintVal, 200);
}

TEST_F(JT_JobScValueTest, CreateStringValue)
{
    JobScValue value(std::string("hello"));
    EXPECT_FALSE(value.IsNull());
    EXPECT_EQ(value.GetType(), JobScValType::String);

    bool ok = false;
    std::string strVal = value.ToString(ok);
    EXPECT_TRUE(ok);
    EXPECT_EQ(strVal, "hello");
}

TEST_F(JT_JobScValueTest, CreateBlobValue)
{
    std::vector<uint8_t> data = {1, 2, 3, 4, 5};
    JobScValue value(data);
    EXPECT_FALSE(value.IsNull());
    EXPECT_EQ(value.GetType(), JobScValType::Blob);

    const std::vector<uint8_t>& blob = value.ToBlob();
    EXPECT_EQ(blob.size(), 5);
    EXPECT_EQ(blob[0], 1);
    EXPECT_EQ(blob[4], 5);
}

TEST_F(JT_JobScValueTest, CopyAndMoveSemantics)
{
    JobScValue original(std::string("test"));

    JobScValue copy(original);
    EXPECT_EQ(copy.GetType(), JobScValType::String);
    bool ok = false;
    EXPECT_EQ(copy.ToString(ok), "test");

    JobScValue moved(std::move(copy));
    EXPECT_EQ(moved.GetType(), JobScValType::String);
}

class JT_JobScResultTest : public ::testing::Test
{
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(JT_JobScResultTest, ResultEnumValues)
{
    EXPECT_EQ(static_cast<int>(JobScResult::Success), 0);
    EXPECT_EQ(static_cast<int>(JobScResult::Failed), 1);
    EXPECT_EQ(static_cast<int>(JobScResult::MaxCountReached), 2);
    EXPECT_EQ(static_cast<int>(JobScResult::NoExist), 3);
    EXPECT_EQ(static_cast<int>(JobScResult::AlreadyExist), 4);
    EXPECT_EQ(static_cast<int>(JobScResult::InvalidParam), 5);
}

TEST_F(JT_JobScResultTest, PageResultStructure)
{
    JobScPageResult result;
    result.total_count = 100;
    result.total_page = 10;
    result.page_index = 2;
    result.page_size = 10;

    EXPECT_EQ(result.total_count, 100);
    EXPECT_EQ(result.total_page, 10);
    EXPECT_EQ(result.page_index, 2);
    EXPECT_EQ(result.page_size, 10);
}

class JT_JobScMgrIntegrationTest : public ::testing::Test
{
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(JT_JobScMgrIntegrationTest, AddJobBasicFlow)
{
    JobScItem item;
    item.SetJobType(JobScType::ScanToEmail);
    item.SetDescription("Integration Test Job");
    item.SetAccountId(987654321);

    auto result = JobScMgr::Add(987654321, item);

    EXPECT_EQ(result.first, JobScResult::Success);
    EXPECT_GT(result.second, 0);
}

TEST_F(JT_JobScMgrIntegrationTest, GetListByTypePageFlow)
{
    JobScPageQuery query;
    query.SetType(JobScType::ScanToEmail);
    query.SetPageIndex(0);
    query.SetPageSize(10);

    JobScPageResult page_result;
    std::vector<JobScItem> items;

    auto result = JobScMgr::GetListByTypePage(query, page_result, items);

    EXPECT_EQ(result, JobScResult::Success);
    EXPECT_GE(page_result.total_count, 0);
    EXPECT_GE(page_result.total_page, 0);
}

TEST_F(JT_JobScMgrIntegrationTest, UpdateJobFlow)
{
    auto addResult = JobScMgr::Add(111222333, JobScItem());
    EXPECT_EQ(addResult.first, JobScResult::Success);

    JobScItem updateItem;
    updateItem.SetJobType(JobScType::ScanToFTP);
    updateItem.SetDescription("Updated Job");

    auto updateResult = JobScMgr::Update(addResult.second, updateItem);
    EXPECT_EQ(updateResult, JobScResult::Success);
}

TEST_F(JT_JobScMgrIntegrationTest, DeleteJobFlow)
{
    auto addResult = JobScMgr::Add(444555666, JobScItem());
    EXPECT_EQ(addResult.first, JobScResult::Success);

    std::vector<int64_t> rids = {addResult.second};
    auto deleteResult = JobScMgr::Delete(rids);
    EXPECT_EQ(deleteResult, JobScResult::Success);
}

TEST_F(JT_JobScMgrIntegrationTest, DeleteByTypeFlow)
{
    JobScItem item;
    item.SetJobType(JobScType::ScanToSMB);
    JobScMgr::Add(777888999, item);

    std::vector<JobScType> types = {JobScType::ScanToSMB};
    auto deleteResult = JobScMgr::DeleteByType(types);

    EXPECT_EQ(deleteResult, JobScResult::Success);
}

TEST_F(JT_JobScMgrIntegrationTest, MultipleJobsPerAccount)
{
    const uint64_t accountId = 123123123;

    JobScItem item1;
    item1.SetJobType(JobScType::ScanToEmail);
    item1.SetDescription("Job 1");
    auto result1 = JobScMgr::Add(accountId, item1);
    EXPECT_EQ(result1.first, JobScResult::Success);

    JobScItem item2;
    item2.SetJobType(JobScType::ScanToFTP);
    item2.SetDescription("Job 2");
    auto result2 = JobScMgr::Add(accountId, item2);
    EXPECT_EQ(result2.first, JobScResult::Success);
}

TEST_F(JT_JobScMgrIntegrationTest, PageQueryAllTypes)
{
    JobScPageQuery query;
    query.SetType(JobScType::All);
    query.SetPageIndex(0);
    query.SetPageSize(5);

    JobScPageResult page_result;
    std::vector<JobScItem> items;

    auto result = JobScMgr::GetListByTypePage(query, page_result, items);

    EXPECT_EQ(result, JobScResult::Success);
    EXPECT_LE(items.size(), 5);
}

TEST_F(JT_JobScMgrIntegrationTest, ObserverPattern)
{
    int eventCount = 0;
    JobScObserver observer = [&eventCount](JobScEventType event) {
        eventCount++;
    };

    JobScMgr::AddObserver(observer);

    JobScItem item;
    item.SetJobType(JobScType::ScanToUSB);
    JobScMgr::Add(555666777, item);

    JobScMgr::RemoveObserver(observer);

    EXPECT_GT(eventCount, 0);
}

TEST_F(JT_JobScMgrIntegrationTest, PaginationBoundary)
{
    JobScPageQuery query;
    query.SetType(JobScType::All);
    query.SetPageIndex(999);
    query.SetPageSize(10);

    JobScPageResult page_result;
    std::vector<JobScItem> items;

    auto result = JobScMgr::GetListByTypePage(query, page_result, items);

    EXPECT_EQ(result, JobScResult::Success);
    EXPECT_EQ(items.size(), 0);
}

TEST_F(JT_JobScMgrIntegrationTest, DifferentJobTypesFlow)
{
    std::vector<JobScType> types = {
        JobScType::ScanToEmail,
        JobScType::ScanToFTP,
        JobScType::ScanToSMB,
        JobScType::ScanToUSB};

    for (auto type : types)
    {
        JobScItem item;
        item.SetJobType(type);
        item.SetDescription("Test " + std::to_string(static_cast<int>(type)));

        auto result = JobScMgr::Add(111111111, item);
        EXPECT_EQ(result.first, JobScResult::Success);
    }
}

class JT_JobScEdgeCaseTest : public ::testing::Test
{
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(JT_JobScEdgeCaseTest, EmptyDescription)
{
    JobScItem item;
    item.SetDescription("");
    item.SetJobType(JobScType::ScanToEmail);

    auto result = JobScMgr::Add(222222222, item);
    EXPECT_EQ(result.first, JobScResult::Success);
}

TEST_F(JT_JobScEdgeCaseTest, EmptySettings)
{
    JobScItem item;
    item.SetDescription("Test");
    item.SetSettings({});

    auto result = JobScMgr::Add(333333333, item);
    EXPECT_EQ(result.first, JobScResult::Success);
}

TEST_F(JT_JobScEdgeCaseTest, LargePageSize)
{
    JobScPageQuery query;
    query.SetType(JobScType::All);
    query.SetPageSize(1000);

    JobScPageResult page_result;
    std::vector<JobScItem> items;

    auto result = JobScMgr::GetListByTypePage(query, page_result, items);
    EXPECT_EQ(result, JobScResult::Success);
}

TEST_F(JT_JobScEdgeCaseTest, ZeroPageSize)
{
    JobScPageQuery query;
    query.SetType(JobScType::All);
    query.SetPageSize(0);

    JobScPageResult page_result;
    std::vector<JobScItem> items;

    auto result = JobScMgr::GetListByTypePage(query, page_result, items);
    EXPECT_EQ(result, JobScResult::Failed);
}

TEST_F(JT_JobScEdgeCaseTest, DeleteNonExistRid)
{
    std::vector<int64_t> rids = {999999999};
    auto result = JobScMgr::Delete(rids);
    EXPECT_EQ(result, JobScResult::Success);
}

TEST_F(JT_JobScEdgeCaseTest, UpdateNonExistRid)
{
    JobScItem item;
    item.SetDescription("Test");
    auto result = JobScMgr::Update(999999999, item);
    EXPECT_EQ(result, JobScResult::Success);
}

TEST_F(JT_JobScEdgeCaseTest, OrderTypeAscDesc)
{
    JobScPageQuery queryAsc;
    queryAsc.SetOrderType(JobScOrderType::ASC);

    JobScPageQuery queryDesc;
    queryDesc.SetOrderType(JobScOrderType::DESC);

    JobScPageResult resultAsc, resultDesc;
    std::vector<JobScItem> itemsAsc, itemsDesc;

    JobScMgr::GetListByTypePage(queryAsc, resultAsc, itemsAsc);
    JobScMgr::GetListByTypePage(queryDesc, resultDesc, itemsDesc);

    EXPECT_EQ(resultAsc.total_count, resultDesc.total_count);
}
