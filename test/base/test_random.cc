#include <gtest/gtest.h>
#include <iostream>
#include "random/random.h"

class test_Random : public testing::Test {
 public:
  test_Random() {}
  ~test_Random() {}

  virtual void SetUp(void) {}

  virtual void TearDown(void) {}
};

TEST(test_Random, RandomSeed) {
  uint32_t prev = 0;
  uint32_t seed = 0;
  for (int i = 0; i < 2048; ++i) {
    EXPECT_NE(prev, seed = kingfisher::random::Random::RandomNumberSeed());
    prev = seed;
  }
}
