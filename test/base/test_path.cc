#include <gtest/gtest.h>

#include <iostream>

#include "path/path.h"

using namespace kingfisher;
using namespace kingfisher::path;

class test_Path : public testing::Test {
 public:
  test_Path() {}
  ~test_Path() {}

  virtual void SetUp(void) {}

  virtual void TearDown(void) {}
};

TEST_F(test_Path, Abs) {
  std::string filepath = "../../.";
  std::string abs_path = Abs(filepath);

  std::cout << "absolute path: " << abs_path << std::endl;
}
