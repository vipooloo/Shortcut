#include "JobScMgr.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class JT_JobScAdd : public ::testing::Test
{
  protected:
    void SetUp() override {}
    void TearDown() override {}
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
};


TEST_F(JT_JobScAdd, Add)
{
    JobScItem item;
    EXPECT_EQ(item.GetRid(), INVALID_RID);
    EXPECT_EQ(item.GetAccountId(), 0);
    EXPECT_EQ(item.GetJobType(), JobScType::None);
    EXPECT_EQ(item.GetDescription(), "");
    EXPECT_TRUE(item.GetSettings().empty());
    EXPECT_TRUE(item.GetAddressList().empty());
}
