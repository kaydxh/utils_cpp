#include <gtest/gtest.h>

#include <iostream>

#include "core/scope_guard.h"
#include "file/file.h"
#include "file/file_util.h"

using namespace kingfisher;
using namespace kingfisher ::fileutil;

class test_FileUtil : public testing::Test {
 public:
  test_FileUtil() {}
  ~test_FileUtil() {}

  virtual void SetUp(void) {}

  virtual void TearDown(void) {}
};

TEST_F(test_FileUtil, CopyFile) {
  EXPECT_TRUE(CopyFile("/etc/hosts", "./hosts"));
  EXPECT_FALSE(CopyFile("/etc/host", "./host"));
}

TEST_F(test_FileUtil, DeleteFile) { EXPECT_TRUE(DeleteFile("./hosts")); }

TEST_F(test_FileUtil, IsDir) { EXPECT_TRUE(IsDir("/etc")); }

TEST_F(test_FileUtil, WriteFile) {
  std::string filename = "./tmp/test/meta.test";
  for (int i = 0; i < 5; ++i) {
    std::string content = "hello word\n";
    auto n = WriteFile(filename, content, true);
    std::cout << n << std::endl;
    EXPECT_EQ(n, content.length());
  }
}

TEST_F(test_FileUtil, MakeDirAll) {
  std::string dir = "./tmp/";
  int ret = MakeDirAll(dir);
  EXPECT_EQ(ret, 0);
}
