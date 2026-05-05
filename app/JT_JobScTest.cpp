#include "JobScMgr.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class JobScItemTest : public ::testing::Test
{
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(JobScItemTest, DefaultConstructor)
{
    JobScItem item;
    EXPECT_EQ(item.GetRid(), INVALID_RID);
    EXPECT_EQ(item.GetAccountId(), 0);
    EXPECT_EQ(item.GetJobType(), JobScType::None);
    EXPECT_EQ(item.GetDescription(), "");
    EXPECT_TRUE(item.GetSettings().empty());
    EXPECT_TRUE(item.GetAddressList().empty());
}

TEST_F(JobScItemTest, SettersAndGetters)
{
    JobScItem item;
    int64_t rid = 100;
    uint64_t account_id = 200;
    JobScType job_type = JobScType::ScanToEmail;
    std::string description = "Test Job";
    std::vector<uint8_t> settings = {0x01, 0x02, 0x03};
    std::vector<uint8_t> address_list = {0x04, 0x05};

    item.SetRid(rid);
    item.SetAccountId(account_id);
    item.SetJobType(job_type);
    item.SetDescription(description);
    item.SetSettings(settings);
    item.SetAddressList(address_list);

    EXPECT_EQ(item.GetRid(), rid);
    EXPECT_EQ(item.GetAccountId(), account_id);
    EXPECT_EQ(item.GetJobType(), job_type);
    EXPECT_EQ(item.GetDescription(), description);
    EXPECT_EQ(item.GetSettings(), settings);
    EXPECT_EQ(item.GetAddressList(), address_list);
}

TEST_F(JobScItemTest, MoveConstructor)
{
    JobScItem item;
    item.SetRid(100);
    item.SetDescription("Test");

    JobScItem moved_item(std::move(item));
    EXPECT_EQ(moved_item.GetRid(), 100);
    EXPECT_EQ(moved_item.GetDescription(), "Test");
}

TEST_F(JobScItemTest, MoveAssignment)
{
    JobScItem item;
    item.SetRid(100);
    item.SetDescription("Test");

    JobScItem moved_item;
    moved_item = std::move(item);
    EXPECT_EQ(moved_item.GetRid(), 100);
    EXPECT_EQ(moved_item.GetDescription(), "Test");
}

TEST_F(JobScItemTest, SelfMoveAssignment)
{
    JobScItem item;
    item.SetRid(100);
    item.SetDescription("Test");

    item = std::move(item);
    EXPECT_EQ(item.GetRid(), 100);
    EXPECT_EQ(item.GetDescription(), "Test");
}

class JobScPageQueryTest : public ::testing::Test
{
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(JobScPageQueryTest, DefaultConstructor)
{
    JobScPageQuery query;
    EXPECT_EQ(query.GetKeyword(), "");
    EXPECT_EQ(query.GetType(), JobScType::None);
    EXPECT_EQ(query.GetOrderType(), JobScOrderType::ASC);
    EXPECT_EQ(query.GetPageIndex(), JOBSC_PAGE_DEFAULT_INDEX);
    EXPECT_EQ(query.GetPageSize(), JOBSC_PAGE_DEFAULT_SIZE);
}

TEST_F(JobScPageQueryTest, SettersAndGetters)
{
    JobScPageQuery query;
    std::string keyword = "test";
    JobScType type = JobScType::ScanToFTP;
    JobScOrderType order_type = JobScOrderType::DESC;
    uint32_t page_index = 5;
    uint32_t page_size = 20;

    query.SetKeyword(keyword);
    query.SetType(type);
    query.SetOrderType(order_type);
    query.SetPageIndex(page_index);
    query.SetPageSize(page_size);

    EXPECT_EQ(query.GetKeyword(), keyword);
    EXPECT_EQ(query.GetType(), type);
    EXPECT_EQ(query.GetOrderType(), order_type);
    EXPECT_EQ(query.GetPageIndex(), page_index);
    EXPECT_EQ(query.GetPageSize(), page_size);
}

TEST_F(JobScPageQueryTest, AllJobScTypeValues)
{
    EXPECT_EQ(static_cast<int32_t>(JobScType::None), 0);
    EXPECT_EQ(static_cast<int32_t>(JobScType::ScanToEmail), 1);
    EXPECT_EQ(static_cast<int32_t>(JobScType::ScanToFTP), 2);
    EXPECT_EQ(static_cast<int32_t>(JobScType::ScanToSMB), 3);
    EXPECT_EQ(static_cast<int32_t>(JobScType::ScanToUSB), 4);
    EXPECT_EQ(static_cast<int32_t>(JobScType::All), 5);
}

TEST_F(JobScPageQueryTest, AllJobScResultValues)
{
    EXPECT_EQ(static_cast<int32_t>(JobScResult::Success), 0);
    EXPECT_EQ(static_cast<int32_t>(JobScResult::Failed), 1);
    EXPECT_EQ(static_cast<int32_t>(JobScResult::MaxCountReached), 2);
    EXPECT_EQ(static_cast<int32_t>(JobScResult::NoExist), 3);
    EXPECT_EQ(static_cast<int32_t>(JobScResult::AlreadyExist), 4);
    EXPECT_EQ(static_cast<int32_t>(JobScResult::InvalidParam), 5);
}

TEST_F(JobScPageQueryTest, JobScEventTypeValues)
{
    EXPECT_EQ(static_cast<int32_t>(JobScEventType::Added), 0);
    EXPECT_EQ(static_cast<int32_t>(JobScEventType::Updated), 1);
    EXPECT_EQ(static_cast<int32_t>(JobScEventType::Deleted), 2);
}

TEST_F(JobScPageQueryTest, JobScOrderTypeValues)
{
    EXPECT_EQ(static_cast<int32_t>(JobScOrderType::ASC), 0);
    EXPECT_EQ(static_cast<int32_t>(JobScOrderType::DESC), 1);
}

TEST_F(JobScPageQueryTest, Constants)
{
    EXPECT_EQ(JOBSC_PAGE_DEFAULT_INDEX, 0U);
    EXPECT_EQ(JOBSC_PAGE_DEFAULT_SIZE, 10U);
    EXPECT_EQ(INVALID_RID, 0);
}

class JobScObserverTest : public ::testing::Test
{
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(JobScObserverTest, AddAndRemoveObserver)
{
    bool observer_called = false;
    JobScObserver observer = [&](JobScEventType event) {
        observer_called = true;
    };

    JobScMgr::AddObserver(observer);
    JobScMgr::RemoveObserver(observer);
}

TEST_F(JobScObserverTest, MultipleObservers)
{
    int call_count = 0;
    JobScObserver observer1 = [&](JobScEventType event) {
        call_count++;
    };
    JobScObserver observer2 = [&](JobScEventType event) {
        call_count++;
    };

    JobScMgr::AddObserver(observer1);
    JobScMgr::AddObserver(observer2);
    JobScMgr::RemoveObserver(observer1);
    JobScMgr::RemoveObserver(observer2);
}

TEST_F(JobScObserverTest, EmptyObserver)
{
    JobScObserver empty_observer;
    EXPECT_TRUE(empty_observer == nullptr);
    JobScMgr::AddObserver(empty_observer);
    JobScMgr::RemoveObserver(empty_observer);
}

class JobScMgrAddTest : public ::testing::Test
{
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(JobScMgrAddTest, AddJobWithValidParams)
{
    uint64_t account_id = 1;
    JobScType type = JobScType::ScanToEmail;
    std::string description = "Test Add Job";
    std::vector<uint8_t> settings = {0x01};
    std::vector<uint8_t> addr_info = {0x02};

    auto result = JobScMgr::Add(account_id, type, description, settings, addr_info);
    EXPECT_EQ(result.first, JobScResult::Success);
    EXPECT_GE(result.second.GetRid(), 1);
}

TEST_F(JobScMgrAddTest, AddJobWithEmptyDescription)
{
    uint64_t account_id = 1;
    JobScType type = JobScType::ScanToFTP;
    std::string description = "";
    std::vector<uint8_t> settings = {0x01};
    std::vector<uint8_t> addr_info = {0x02};

    auto result = JobScMgr::Add(account_id, type, description, settings, addr_info);
    EXPECT_EQ(result.first, JobScResult::Success);
}

TEST_F(JobScMgrAddTest, AddJobWithEmptySettings)
{
    uint64_t account_id = 1;
    JobScType type = JobScType::ScanToSMB;
    std::string description = "Test";
    std::vector<uint8_t> settings;
    std::vector<uint8_t> addr_info;

    auto result = JobScMgr::Add(account_id, type, description, settings, addr_info);
    EXPECT_EQ(result.first, JobScResult::Success);
}

TEST_F(JobScMgrAddTest, AddJobWithAllTypes)
{
    std::vector<JobScType> types = {
        JobScType::ScanToEmail,
        JobScType::ScanToFTP,
        JobScType::ScanToSMB,
        JobScType::ScanToUSB};

    for (auto type : types)
    {
        auto result = JobScMgr::Add(1, type, "Test", {0x01}, {0x02});
        EXPECT_EQ(result.first, JobScResult::Success);
    }
}

class JobScMgrDeleteTest : public ::testing::Test
{
  protected:
    int64_t test_rid;
    void SetUp() override
    {
        auto result = JobScMgr::Add(1, JobScType::ScanToEmail, "Test Delete", {0x01}, {0x02});
        test_rid = result.second.GetRid();
    }
    void TearDown() override
    {
    }
};

TEST_F(JobScMgrDeleteTest, DeleteExistingJob)
{
    auto result = JobScMgr::Delete({test_rid});
    EXPECT_EQ(result, JobScResult::Success);
}

TEST_F(JobScMgrDeleteTest, DeleteMultipleJobs)
{
    auto result1 = JobScMgr::Add(1, JobScType::ScanToFTP, "Test1", {0x01}, {0x02});
    auto result2 = JobScMgr::Add(1, JobScType::ScanToFTP, "Test2", {0x01}, {0x02});

    std::vector<int64_t> rids = {result1.second.GetRid(), result2.second.GetRid()};
    auto result = JobScMgr::Delete(rids);
    EXPECT_EQ(result, JobScResult::Success);
}

TEST_F(JobScMgrDeleteTest, DeleteNonExistingJob)
{
    auto result = JobScMgr::Delete({99999});
    EXPECT_EQ(result, JobScResult::Success);
}

TEST_F(JobScMgrDeleteTest, DeleteByType)
{
    JobScMgr::Add(1, JobScType::ScanToSMB, "Test", {0x01}, {0x02});
    JobScMgr::Add(1, JobScType::ScanToSMB, "Test2", {0x01}, {0x02});

    auto result = JobScMgr::DeleteByType({JobScType::ScanToSMB});
    EXPECT_EQ(result, JobScResult::Success);
}

TEST_F(JobScMgrDeleteTest, DeleteByMultipleTypes)
{
    JobScMgr::Add(1, JobScType::ScanToEmail, "Test", {0x01}, {0x02});
    JobScMgr::Add(1, JobScType::ScanToFTP, "Test", {0x01}, {0x02});

    auto result = JobScMgr::DeleteByType({JobScType::ScanToEmail, JobScType::ScanToFTP});
    EXPECT_EQ(result, JobScResult::Success);
}

class JobScMgrUpdateTest : public ::testing::Test
{
  protected:
    int64_t test_rid;
    void SetUp() override
    {
        auto result = JobScMgr::Add(1, JobScType::ScanToEmail, "Original", {0x01}, {0x02});
        test_rid = result.second.GetRid();
    }
    void TearDown() override
    {
        JobScMgr::Delete({test_rid});
    }
};

TEST_F(JobScMgrUpdateTest, UpdateExistingJob)
{
    JobScItem item;
    item.SetRid(test_rid);
    item.SetAccountId(1);
    item.SetJobType(JobScType::ScanToFTP);
    item.SetDescription("Updated");
    item.SetSettings({0x02});
    item.SetAddressList({0x03});

    auto result = JobScMgr::Update(test_rid, item);
    EXPECT_EQ(result, JobScResult::Success);
}

TEST_F(JobScMgrUpdateTest, UpdateNonExistingJob)
{
    JobScItem item;
    item.SetRid(99999);
    item.SetAccountId(1);
    item.SetJobType(JobScType::ScanToEmail);
    item.SetDescription("Updated");

    auto result = JobScMgr::Update(99999, item);
    EXPECT_EQ(result, JobScResult::NoExist);
}

TEST_F(JobScMgrUpdateTest, UpdateWithEmptyDescription)
{
    JobScItem item;
    item.SetRid(test_rid);
    item.SetAccountId(1);
    item.SetJobType(JobScType::ScanToEmail);
    item.SetDescription("");

    auto result = JobScMgr::Update(test_rid, item);
    EXPECT_EQ(result, JobScResult::Success);
}

class JobScMgrGetListTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        for (int i = 0; i < 15; i++)
        {
            JobScMgr::Add(1, JobScType::ScanToEmail, "Test" + std::to_string(i), {0x01}, {0x02});
        }
    }
    void TearDown() override
    {
        JobScMgr::DeleteByType({JobScType::ScanToEmail, JobScType::ScanToFTP, JobScType::ScanToSMB, JobScType::ScanToUSB});
    }
};

TEST_F(JobScMgrGetListTest, GetListByTypePage)
{
    JobScPageQuery query;
    query.SetType(JobScType::ScanToEmail);
    query.SetPageIndex(0);
    query.SetPageSize(10);

    JobScPageResult page_result;
    std::vector<JobScItem> items;

    auto result = JobScMgr::GetListByTypePage(query, page_result, items);
    EXPECT_EQ(result, JobScResult::Success);
    EXPECT_GE(page_result.total_count, 10U);
    EXPECT_GE(page_result.total_page, 1U);
    EXPECT_EQ(page_result.page_index, 0U);
    EXPECT_EQ(page_result.page_size, 10U);
}

TEST_F(JobScMgrGetListTest, GetListWithKeyword)
{
    JobScPageQuery query;
    query.SetType(JobScType::ScanToEmail);
    query.SetKeyword("Test");
    query.SetPageIndex(0);
    query.SetPageSize(10);

    JobScPageResult page_result;
    std::vector<JobScItem> items;

    auto result = JobScMgr::GetListByTypePage(query, page_result, items);
    EXPECT_EQ(result, JobScResult::Success);
}

TEST_F(JobScMgrGetListTest, GetListWithDescendingOrder)
{
    JobScPageQuery query;
    query.SetType(JobScType::ScanToEmail);
    query.SetOrderType(JobScOrderType::DESC);
    query.SetPageIndex(0);
    query.SetPageSize(10);

    JobScPageResult page_result;
    std::vector<JobScItem> items;

    auto result = JobScMgr::GetListByTypePage(query, page_result, items);
    EXPECT_EQ(result, JobScResult::Success);
}

TEST_F(JobScMgrGetListTest, GetListWithDifferentPageSize)
{
    JobScPageQuery query;
    query.SetType(JobScType::ScanToEmail);
    query.SetPageIndex(0);
    query.SetPageSize(5);

    JobScPageResult page_result;
    std::vector<JobScItem> items;

    auto result = JobScMgr::GetListByTypePage(query, page_result, items);
    EXPECT_EQ(result, JobScResult::Success);
    EXPECT_EQ(page_result.page_size, 5U);
}

TEST_F(JobScMgrGetListTest, GetListSecondPage)
{
    JobScPageQuery query;
    query.SetType(JobScType::ScanToEmail);
    query.SetPageIndex(1);
    query.SetPageSize(10);

    JobScPageResult page_result;
    std::vector<JobScItem> items;

    auto result = JobScMgr::GetListByTypePage(query, page_result, items);
    EXPECT_EQ(result, JobScResult::Success);
}

TEST_F(JobScMgrGetListTest, GetListWithNoneType)
{
    JobScPageQuery query;
    query.SetType(JobScType::None);
    query.SetPageIndex(0);
    query.SetPageSize(10);

    JobScPageResult page_result;
    std::vector<JobScItem> items;

    auto result = JobScMgr::GetListByTypePage(query, page_result, items);
    EXPECT_EQ(result, JobScResult::Success);
}

class JobScMgrIntegrationTest : public ::testing::Test
{
  protected:
    int64_t test_rid;
    void SetUp() override
    {
        auto result = JobScMgr::Add(1, JobScType::ScanToEmail, "Integration Test", {0x01}, {0x02});
        test_rid = result.second.GetRid();
    }
    void TearDown() override
    {
        JobScMgr::Delete({test_rid});
    }
};

TEST_F(JobScMgrIntegrationTest, AddUpdateGetListDelete)
{
    JobScItem item;
    item.SetRid(test_rid);
    item.SetAccountId(1);
    item.SetJobType(JobScType::ScanToFTP);
    item.SetDescription("Updated Integration");
    item.SetSettings({0x02});
    item.SetAddressList({0x03});
    auto update_result = JobScMgr::Update(test_rid, item);
    EXPECT_EQ(update_result, JobScResult::Success);

    JobScPageQuery query;
    query.SetType(JobScType::ScanToEmail);
    JobScPageResult page_result;
    std::vector<JobScItem> items;
    auto get_result = JobScMgr::GetListByTypePage(query, page_result, items);
    EXPECT_EQ(get_result, JobScResult::Success);

    auto delete_result = JobScMgr::Delete({test_rid});
    EXPECT_EQ(delete_result, JobScResult::Success);
}

TEST_F(JobScMgrIntegrationTest, AddMultipleJobsAndFilterByType)
{
    JobScMgr::Add(1, JobScType::ScanToFTP, "FTP Job 1", {0x01}, {0x02});
    JobScMgr::Add(1, JobScType::ScanToSMB, "SMB Job 1", {0x01}, {0x02});
    JobScMgr::Add(1, JobScType::ScanToUSB, "USB Job 1", {0x01}, {0x02});

    JobScPageQuery query;
    query.SetType(JobScType::ScanToFTP);
    JobScPageResult page_result;
    std::vector<JobScItem> items;
    auto result = JobScMgr::GetListByTypePage(query, page_result, items);
    EXPECT_EQ(result, JobScResult::Success);
    EXPECT_GE(page_result.total_count, 1U);

    JobScMgr::DeleteByType({JobScType::ScanToFTP, JobScType::ScanToSMB, JobScType::ScanToUSB});
}

class JobScMgrEdgeCaseTest : public ::testing::Test
{
  protected:
    void SetUp() override {}
    void TearDown() override
    {
        JobScMgr::DeleteByType({JobScType::ScanToEmail, JobScType::ScanToFTP, JobScType::ScanToSMB, JobScType::ScanToUSB});
    }
};

TEST_F(JobScMgrEdgeCaseTest, AddJobWithLongDescription)
{
    std::string long_desc(1000, 'a');
    auto result = JobScMgr::Add(1, JobScType::ScanToEmail, long_desc, {0x01}, {0x02});
    EXPECT_EQ(result.first, JobScResult::Success);
}

TEST_F(JobScMgrEdgeCaseTest, AddJobWithLargeSettings)
{
    std::vector<uint8_t> large_settings(1000, 0xFF);
    auto result = JobScMgr::Add(1, JobScType::ScanToEmail, "Test", large_settings, {0x02});
    EXPECT_EQ(result.first, JobScResult::Success);
}

TEST_F(JobScMgrEdgeCaseTest, AddJobWithLargeAddressList)
{
    std::vector<uint8_t> large_addr(1000, 0xFF);
    auto result = JobScMgr::Add(1, JobScType::ScanToEmail, "Test", {0x01}, large_addr);
    EXPECT_EQ(result.first, JobScResult::Success);
}

TEST_F(JobScMgrEdgeCaseTest, GetListWithZeroPageSize)
{
    JobScPageQuery query;
    query.SetType(JobScType::ScanToEmail);
    query.SetPageIndex(0);
    query.SetPageSize(0);

    JobScPageResult page_result;
    std::vector<JobScItem> items;

    auto result = JobScMgr::GetListByTypePage(query, page_result, items);
    EXPECT_EQ(result, JobScResult::Success);
}

TEST_F(JobScMgrEdgeCaseTest, GetListWithLargePageIndex)
{
    JobScPageQuery query;
    query.SetType(JobScType::ScanToEmail);
    query.SetPageIndex(1000);
    query.SetPageSize(10);

    JobScPageResult page_result;
    std::vector<JobScItem> items;

    auto result = JobScMgr::GetListByTypePage(query, page_result, items);
    EXPECT_EQ(result, JobScResult::Success);
}

TEST_F(JobScMgrEdgeCaseTest, DeleteEmptyList)
{
    std::vector<int64_t> empty_list;
    auto result = JobScMgr::Delete(empty_list);
    EXPECT_EQ(result, JobScResult::Success);
}

TEST_F(JobScMgrEdgeCaseTest, DeleteByEmptyTypeList)
{
    std::vector<JobScType> empty_types;
    auto result = JobScMgr::DeleteByType(empty_types);
    EXPECT_EQ(result, JobScResult::Success);
}

TEST_F(JobScMgrEdgeCaseTest, UpdateWithEmptySettings)
{
    auto result = JobScMgr::Add(1, JobScType::ScanToEmail, "Test", {}, {});
    int64_t rid = result.second.GetRid();

    JobScItem item;
    item.SetRid(rid);
    item.SetAccountId(1);
    item.SetJobType(JobScType::ScanToEmail);
    item.SetDescription("Updated");
    item.SetSettings({});
    item.SetAddressList({});

    auto update_result = JobScMgr::Update(rid, item);
    EXPECT_EQ(update_result, JobScResult::Success);

    JobScMgr::Delete({rid});
}

class JobScPageResultTest : public ::testing::Test
{
  protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(JobScPageResultTest, PageResultStructure)
{
    JobScPageResult result;
    result.total_count = 100;
    result.total_page = 10;
    result.page_index = 1;
    result.page_size = 10;

    EXPECT_EQ(result.total_count, 100U);
    EXPECT_EQ(result.total_page, 10U);
    EXPECT_EQ(result.page_index, 1U);
    EXPECT_EQ(result.page_size, 10U);
}

TEST_F(JobScPageResultTest, PageResultZeroInit)
{
    JobScPageResult result = {0, 0, 0, 0};
    EXPECT_EQ(result.total_count, 0U);
    EXPECT_EQ(result.total_page, 0U);
    EXPECT_EQ(result.page_index, 0U);
    EXPECT_EQ(result.page_size, 0U);
}