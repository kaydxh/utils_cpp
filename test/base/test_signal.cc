#include <gtest/gtest.h>

#include <iostream>

#include "os/signal/signal.h"

using namespace kingfisher;
using namespace kingfisher::os;

class test_SignalHandler : public testing::Test {
 public:
  test_SignalHandler() {}
  ~test_SignalHandler() {}

  virtual void SetUp(void) {}

  virtual void TearDown(void) {}
};

TEST_F(test_SignalHandler, SetSignal) {
  auto sigHandler = [](int signum) {
    std::cout << "receive signal: " << signum << std::endl;
    raise(signum);
  };
  auto ret = SignalHandler::SetSignal(SIGINT, sigHandler);
  ASSERT_TRUE(0 == ret);
}
