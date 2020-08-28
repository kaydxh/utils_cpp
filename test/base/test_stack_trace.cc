#include <gtest/gtest.h>
#include <iostream>
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

TEST_F(test_Dump, All) {
  StackTrace st;
  auto stack_trace_info = st.GetStackTrace();
  std::cout << stack_trace_info << std::endl;
}
