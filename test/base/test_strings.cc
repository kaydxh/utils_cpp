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

TEST_F(test_Strings, ParseInt64) {
  struct TestCases {
    std::string str;
    int64_t expected;
  };

  std::vector<TestCases> testCases = {{"123", 123}, {"-123", -123}};

  for (auto testCase : testCases) {
    int64_t result;
    auto ret = ParseInt64(result, testCase.str);
    ASSERT_TRUE(ret == 0);
    std::cout << "result of ParseInt64: " << result << std::endl;
    ASSERT_TRUE(result == testCase.expected);
  }
}

TEST_F(test_Strings, HasPrefix) {
  struct TestCases {
    std::string str;
    std::string prefix;
    int64_t expected;
  };

  std::vector<TestCases> testCases = {{"abcde", "ab", true},
                                      {"abce", "abced", false}};

  for (auto testCase : testCases) {
    auto has = HasPrefix(testCase.str, testCase.prefix);
    ASSERT_TRUE(has == testCase.expected);
    std::cout << "result of HasPrefix: " << has << std::endl;
    ASSERT_TRUE(has == testCase.expected);
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
