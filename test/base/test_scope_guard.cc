#include <gtest/gtest.h>
#include "core/scope_guard.h"

using namespace kingfisher;
using namespace kingfisher::core;

class test_ScopeGuard : public testing::Test {
 public:
  test_ScopeGuard() {}
  ~test_ScopeGuard() {}

  virtual void SetUp(void) {}

  virtual void TearDown(void) {}
};

TEST_F(test_ScopeGuard, All) {
  int x = 0;
  {
    SCOPE_EXIT { ++x; };
    EXPECT_EQ(0, x);
  }
  EXPECT_EQ(1, x);
}
