#include <gtest/gtest.h>
#include "file/file.h"

using namespace kingfisher;
using namespace kingfisher::file;

class test_File : public testing::Test {
 public:
  test_File() {}
  ~test_File() {}

  virtual void SetUp(void) {}

  virtual void TearDown(void) {}
};

TEST_F(test_File, Open) {
  File f;
  EXPECT_EQ(-1, f.GetFd());
}
