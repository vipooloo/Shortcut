#include "JobScMgr.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>

class UT_Example : public ::testing::Test
{
  protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {}
    static void SetUpTestSuite()
    {
    }
    static void TearDownTestSuite()
    {
    }
};

TEST_F(UT_Example, test1)
{
    JobScMgr::Add(123456789, JobScItem());

    JobScPageQuery query;
    JobScPageResult page_result;
    std::vector<JobScItem> items;
    query.SetType(JobScType::All);
    JobScMgr::GetListByTypePage(query, page_result, items);
}