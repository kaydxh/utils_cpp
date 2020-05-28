#include <gtest/gtest.h>
#include <ctime>
#include <iostream>
#include "time/date.h"

using namespace kingfisher;
using namespace kingfisher::time;

class test_Date : public testing::Test {
 public:
  test_Date() {}
  ~test_Date() {}

  virtual void SetUp(void) {}

  virtual void TearDown(void) {}
};

TEST_F(test_Date, All) {
  Date date;
  ASSERT_FALSE(date.Valid());

  Date::YearMonthDay ymd;
  ymd.year = 2020;
  ymd.month = 5;
  ymd.day = 27;

  Date dt(ymd.year, ymd.month, ymd.day);
  std::cout << dt.ToString() << std::endl;
  date.Swap(dt);  // date: 2020-5-27
  std::cout << date.ToString() << std::endl;

  ASSERT_TRUE(date.Valid());
  ASSERT_FALSE(dt.Valid());

  ASSERT_TRUE(2020 == date.Year());
  ASSERT_TRUE(5 == date.Month());
  ASSERT_TRUE(27 == date.Day());

  ASSERT_TRUE(3 == date.WeekDay());
  ASSERT_TRUE("2020-05-27" == date.ToString());

  struct tm stm;
  time_t tt = std::time(nullptr);
  ::gmtime_r(&tt, &stm);
  dt = Date(stm);
  dt = Date(2020, 6, 28);
  ASSERT_TRUE(dt.Year() == date.Year());
  ASSERT_TRUE(dt.Month() > date.Month());
  ASSERT_TRUE(dt.Day() > date.Day());
  ASSERT_TRUE(dt != date);
  ASSERT_TRUE(Date(2020, 5, 26) < date);

  Date::YearMonthDay ymd_tmp = dt.GetYearMonthDay();
  ASSERT_TRUE(ymd_tmp.year == ymd.year);
  ASSERT_TRUE(ymd_tmp.month != ymd.month);
  ASSERT_TRUE(ymd_tmp.day != ymd.day);

  Date dtt(date.GetJulianDayNumber());
  ASSERT_TRUE(dtt == date);
  ASSERT_TRUE(dtt.Year() == date.Year());
  ASSERT_TRUE(dtt.Month() == date.Month());
  ASSERT_TRUE(dtt.Day() == date.Day());
}
