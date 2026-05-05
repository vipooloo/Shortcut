

#include "MockJobScDbAccess.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#define private public
#define protected public
#include "JobScImpl.h"
#include "JobScMgr.h"
#include "JobScSqlDefines.h"

using namespace ::testing;

class JT_JobScQuery : public ::testing::Test
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
    std::pair<JobScResult, JobScItem> AddRecord(JobScType type, uint32_t i)
    {
        auto observer = [](JobScEventType event) {
            EXPECT_EQ(event, JobScEventType::Added);
        };
        JobScMgr::AddObserver(observer);
        std::vector<uint8_t> job_setting{0x01, 0x02};
        std::vector<uint8_t> address_list{0x11, 0x22};
        std::pair<JobScResult, JobScItem> result = JobScMgr::Add(1234, type, "test" + std::to_string(i), job_setting, address_list);

        JobScMgr::RemoveObserver(observer);
        return result;
    }
};

TEST_F(JT_JobScQuery, Query)
{
    auto observer = [](JobScEventType event) {
        EXPECT_EQ(event, JobScEventType::Added);
    };
    JobScMgr::AddObserver(observer);
    for (uint32_t i = 0; i < 10; i++)
    {
        AddRecord(JobScType::ScanToEmail, i);
        AddRecord(JobScType::ScanToFTP, i);
    }
    {
        JobScPageQuery page_query;
        page_query.SetKeyword("abc");
        JobScPageResult out_result;
        std::vector<JobScItem> out_items;
        auto result = JobScMgr::Query(page_query, out_result, out_items);
        EXPECT_EQ(result, JobScResult::Success);
    }
    {
        JobScPageQuery page_query;
        page_query.SetKeyword("es");
        JobScPageResult out_result;
        std::vector<JobScItem> out_items;
        auto result = JobScMgr::Query(page_query, out_result, out_items);
        EXPECT_EQ(result, JobScResult::Success);
        EXPECT_EQ(out_items.size(), 10);
        EXPECT_EQ(out_result.total_count, 20);
        EXPECT_EQ(out_result.total_page, 2);
        EXPECT_EQ(out_result.page_index, 0);
        EXPECT_EQ(out_result.page_size, 10);
    }
    {
        JobScPageQuery page_query;
        page_query.SetKeyword("test1");
        JobScPageResult out_result;
        std::vector<JobScItem> out_items;
        auto result = JobScMgr::Query(page_query, out_result, out_items);
        EXPECT_EQ(result, JobScResult::Success);
        EXPECT_EQ(out_items.size(), 2);
        EXPECT_EQ(out_result.total_count, 2);
        EXPECT_EQ(out_result.total_page, 1);
        EXPECT_EQ(out_result.page_index, 0);
        EXPECT_EQ(out_result.page_size, 10);
    }
    {
        JobScPageQuery page_query;
        page_query.SetKeyword("test1");
        page_query.SetType(JobScType::ScanToEmail);
        JobScPageResult out_result;
        std::vector<JobScItem> out_items;
        auto result = JobScMgr::Query(page_query, out_result, out_items);
        EXPECT_EQ(result, JobScResult::Success);
        EXPECT_EQ(out_items.size(), 1);
        EXPECT_EQ(out_result.total_count, 1);
        EXPECT_EQ(out_result.total_page, 1);
        EXPECT_EQ(out_result.page_index, 0);
        EXPECT_EQ(out_result.page_size, 10);
    }
    {
        JobScPageQuery page_query;
        page_query.SetKeyword("adsfad");
        page_query.SetType(JobScType::ScanToEmail);
        JobScPageResult out_result;
        std::vector<JobScItem> out_items;
        auto result = JobScMgr::Query(page_query, out_result, out_items);
        EXPECT_EQ(result, JobScResult::Success);
        EXPECT_EQ(out_items.size(), 0);
        EXPECT_EQ(out_result.total_count, 0);
        EXPECT_EQ(out_result.total_page, 0);
        EXPECT_EQ(out_result.page_index, 0);
        EXPECT_EQ(out_result.page_size, 10);
    }
    JobScMgr::RemoveObserver(observer);
}
