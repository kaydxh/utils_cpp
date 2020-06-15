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
  TimerEvent<Callback> event([&count]() {
    std::cout << "callback event at time: " << getJiffies() << std::endl;
    ++count;
  });
  TimerEvent<Callback> event2([&count]() {
    std::cout << "callback event2 at time: " << getJiffies() << std::endl;
    ++count;
  });

#if 0
  timers.advance(10);
  EXPECT_EQ(count, 0);
  EXPECT_TRUE(!event.active());

  timers.schedule(&event, 5);
  EXPECT_TRUE(event.active());
  timers.advance(5);
  EXPECT_EQ(count, 1);

  timers.advance(255);
  EXPECT_EQ(count, 1);
#endif

  timers.Schedule(&event, 100);
  timers.Schedule(&event2, 200);
  // timers.Schedule(&event, 300);
  timers.Start();
  sleep(10);

  EXPECT_EQ(count, 2);
}
