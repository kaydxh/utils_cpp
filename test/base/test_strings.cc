#include <gtest/gtest.h>

#include <iostream>

#include "strings/strings.h"

using namespace kingfisher;
using namespace kingfisher::strings;

class test_Strings : public testing::Test {
 public:
  test_Strings() {}
  ~test_Strings() {}

  virtual void SetUp(void) {}

  virtual void TearDown(void) {}
};

TEST_F(test_Strings, All) {
  std::string filepath = "./tmp/test";
  std::vector<std::string> path_nodes = Split(filepath, '/');

  std::cout << "path nodes: " << std::endl;
  for (auto &node : path_nodes) {
    std::cout << node << std::endl;
  }
}
