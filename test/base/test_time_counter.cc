#include <gtest/gtest.h>

#include <iostream>

#include "time/time_counter.h"

using namespace kingfisher;
using namespace kingfisher::time;

class test_TimeCounter : public testing::Test {
 public:
  test_TimeCounter() {}
  ~test_TimeCounter() {}

  virtual void SetUp(void) {}

  virtual void TearDown(void) {}
};

TEST_F(test_TimeCounter, Tick) {
  TimeCounter tc;
  sleep(1);
  tc.Tick("sleep 1");
  sleep(2);
  tc.Tick("sleep 2");
  sleep(3);
  tc.Tick("sleep 3");
  std::cout << tc.String() << std::endl;
}

