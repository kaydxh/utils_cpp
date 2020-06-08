#include <gtest/gtest.h>
#include <iostream>
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
  // File f2("/etc/hosts");
  File f2("/etc/vimrc");
  EXPECT_NE(-1, f2.GetFd());
  size_t f2_size = f2.GetFileSize();
  std::cout << "f2_size :" << f2_size << std::endl;
  auto str = f2.ReadBytes(10);
  // auto str = f2.ReadUInt8();
  std::cout << "read content:" << str << std::endl;
  auto f2_pos = f2.GetPositon();
  std::cout << "f2_pos :" << f2_pos << std::endl;
  // auto tt = ftell(f2.GetFile());

  // std::cout << "tt_pos :" << tt << std::endl;

  File tmp = File::temporary();
  int tmp_fd = tmp.GetFd();
  std::cout << "tmp_fd: " << tmp_fd << std::endl;
  EXPECT_NE(-1, tmp_fd);

  tmp.Swap(f2);
#if 0
  str = tmp.ReadBytes(10);
  std::cout << "read content:" << str << std::endl;
  size_t tmp_size = tmp.GetFileSize();
  EXPECT_EQ(f2_size, tmp_size);
  std::cout << "tmp_size :" << tmp_size << std::endl;
  auto tmp_pos = tmp.GetPositon();
  std::cout << "tmp_pos  :" << tmp_pos << std::endl;
#endif
}
