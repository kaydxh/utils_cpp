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
  std::string filepath = ".";
  std::string abs_path = Abs(filepath);
  ASSERT_TRUE(IsAbs(abs_path));
  std::cout << "absolute path: " << abs_path << std::endl;
}

TEST_F(test_Path, IsAbs) {
  std::string filepath = ".";
  ASSERT_FALSE(IsAbs(filepath));
}

TEST_F(test_Path, Dir) {
  std::string filepath = "/data/home/image/1.jpg";
  std::string dir = Dir(filepath);
  ASSERT_TRUE(dir == "/data/home/image/");
}

TEST_F(test_Path, Split) {
  std::string filepath = "/data/home/image/1.jpg";
  std::string dir;
  std::string filename;
  int ret = Split(filepath, dir, filename);
  ASSERT_TRUE(ret == 0);
  ASSERT_TRUE(dir == "/data/home/image/");
  ASSERT_TRUE(filename == "1.jpg");
  std::cout << "dir: " << dir << ", file:" << filename << std::endl;
}

