#include <gtest/gtest.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include "dump/stack_trace.h"

using namespace kingfisher;
using namespace kingfisher::dump;

class test_Dump : public testing::Test {
 public:
  test_Dump() {}
  ~test_Dump() {}

  virtual void SetUp(void) {}

  virtual void TearDown(void) {}
};

TEST_F(test_Dump, Single) {
  StackTrace::InstallFailureSignalHandler();
#if 0
  char *p = nullptr;
  *p = 'a';
#endif
}

TEST_F(test_Dump, MulitThreads) {
  StackTrace::InstallFailureSignalHandler();
  constexpr size_t kNumThreads = 10;
  std::vector<std::thread> threads;
  threads.resize(kNumThreads);

  std::chrono::seconds sec(1);

  for (auto &t : threads) {
    t = std::thread([=]() {
      std::this_thread::sleep_for(sec);
      char *p = nullptr;
      *p = 'a';
    });
  }

  std::thread t_crash([]() {
#if 0
    char *p = nullptr;
    *p = 'a';
#endif
  });

  for (auto &t : threads) {
    t.join();
  }
  t_crash.join();
}
