#include <gtest/gtest.h>
#include "time/timestamp.h"
using namespace kingfisher;
using namespace kingfisher::time;

class test_Timestamp : public testing::Test {
 public:
  test_Timestamp() {}
  ~test_Timestamp() {}

  virtual void SetUp(void) {}

  virtual void TearDown(void) {}
};

TEST(test_Timestamp, All) {
  Timestamp ts;
  ASSERT_FALSE(ts.Valid());

  Timestamp now = Timestamp::Now();
  ASSERT_TRUE(now.Valid());

  now.Swap(ts);
  ASSERT_FALSE(now.Valid());
  ASSERT_TRUE(ts.Valid());

  Timestamp tt(ts.MicroSecondsSinceEpoch());
  ASSERT_TRUE(tt == ts);
  ASSERT_TRUE(tt.ToString() == ts.ToString());
  ASSERT_TRUE(tt.ToFormattedString() == ts.ToFormattedString());
}
