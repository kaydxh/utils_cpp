#include <gtest/gtest.h>

#include "net/ip.h"

using namespace kingfisher;
using namespace kingfisher::net;

class test_IP : public testing::Test {
 public:
  test_IP() {}
  ~test_IP() {}

  virtual void SetUp(void) {}

  virtual void TearDown(void) {}
};

TEST_F(test_IP, GetHostIP) {
  std::string ip;
  int ret = IPAddress::GetHostIP(ip);
  EXPECT_EQ(ret, 0);
  EXPECT_TRUE(ip.size() > 0);
  std::cout << "ip: " << ip << std::endl;
}
