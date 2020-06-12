#include <gtest/gtest.h>
#include <iostream>

#include <functional>
#include "time/timer.h"

using namespace kingfisher;
using namespace kingfisher::time;

class test_Timer : public testing::Test {
 public:
  test_Timer() {}
  ~test_Timer() {}

  virtual void SetUp(void) {}

  virtual void TearDown(void) {}
};

TEST_F(test_Timer, ALL) {
  using Callback = std::function<void()>;
  TimerWheel timers;
  int count = 0;
  TimerEvent<Callback> event([&count]() { ++count; });

  timers.scheduleRepeating(&event, 5);
}
