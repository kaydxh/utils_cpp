#include <gtest/gtest.h>
#include "utils/guid.h"

using namespace kingfisher;
using namespace kingfisher::utils;

class test_Guid : public testing::Test {
 public:
  test_Guid() {}
  ~test_Guid() {}

  virtual void SetUp(void) {}

  virtual void TearDown(void) {}
};

TEST(test_Guid, Guid) {
  std::string guid1;
  std::string guid2;
  for (auto i = 0; i < 100; ++i) {
    guid1 = Guid::GenUuidString();
    guid2 = Guid::GenUuidString();
    //    std::cout << guid1 << " " << guid1.length() << std::endl;
    //   std::cout << guid2 << " " << guid2.length() << std::endl;
    EXPECT_NE(guid1, guid2);
    EXPECT_EQ(guid1.length(), 36);
    EXPECT_EQ(guid2.length(), 36);
  }
}
