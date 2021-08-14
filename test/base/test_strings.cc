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

TEST_F(test_Strings, IsNumber) {
  struct TestCases {
    std::string str;
    bool expected;
  };

  std::vector<TestCases> testCases = {{"123", true}, {"12a", false}};

  for (auto testCase : testCases) {
    auto isNum = IsNumber(testCase.str);
    std::cout << "isNum: " << isNum << std::endl;
    ASSERT_TRUE(isNum == testCase.expected);
  }
}

TEST_F(test_Strings, All) {
  std::string filepath = "./tmp/test";
  std::vector<std::string> path_nodes = Split(filepath, '/');

  std::cout << "path nodes: " << std::endl;
  for (auto &node : path_nodes) {
    std::cout << node << std::endl;
  }
}
