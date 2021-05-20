
#include <gtest/gtest.h>

#include "os/getwd.h"

using namespace kingfisher;
using namespace kingfisher::os;

class test_Getwd : public testing::Test {
 public:
  test_Getwd() {}
  ~test_Getwd() {}

  virtual void SetUp(void) {}

  virtual void TearDown(void) {}
};

TEST(test_Getwd, Getwd) {
  std::string path;
  path = Getwd();
  std::cout << "get work dir: " << path << std::endl;
  EXPECT_NE(path, "");
}
