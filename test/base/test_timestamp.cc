#include <gtest/gtest.h>

#include <iostream>

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
  std::cout << ts.ToString() << std::endl;
  std::cout << ts.ToFormattedString() << std::endl;
  std::cout << ts.ToSecDotMicroString() << std::endl;

  Timestamp tt(ts.MicroSecondsSinceEpoch());
  ASSERT_TRUE(tt == ts);
  ASSERT_TRUE(tt.ToString() == ts.ToString());
  ASSERT_TRUE(tt.ToFormattedString() == ts.ToFormattedString());
  ASSERT_TRUE(tt.ToFormattedString(false) == ts.ToFormattedString(false));
  ASSERT_TRUE(tt.ToSecDotMicroString() == ts.ToSecDotMicroString());

  ASSERT_FALSE(ts < tt);
  ASSERT_TRUE(0 == DiffTimerenceSecond(ts, tt));

  Timestamp add_time = Timestamp::Invalid();
  add_time = AddTime(ts, 1000.0);
  std::cout << add_time.ToString() << std::endl;
  std::cout << add_time.ToFormattedString() << std::endl;

  std::cout << "diff (ts-tt) time: " << DiffTimerenceMilliSecond(add_time, tt)
            << "ms" << std::endl;
  ASSERT_TRUE(1000.0 == DiffTimerenceSecond(add_time, tt));
}
