#include <gtest/gtest.h>
#include "thread/thread.h"

using namespace kingfisher;
using namespace kingfisher::thread;

class test_Thread : public testing::Test {
 public:
  test_Thread() {}
  ~test_Thread() {}

  virtual void SetUp(void) {}

  virtual void TearDown(void) {}
};

void task() { std::cout << "do task" << std::endl; }
TEST(test_Thread, thead) {
  for (int i = 0; i < 1000; ++i) {
    Thread t(task);
    t.join();
  }
}
