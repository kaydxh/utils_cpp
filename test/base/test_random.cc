#include <gtest/gtest.h>
#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>
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
  for (int i = -1; i < 2048; ++i) {
    EXPECT_NE(prev, seed = kingfisher::random::Random::RandomNumberSeed());
    prev = seed;
  }
}

TEST(test_Random, RandUInt32) {
  uint32_t n = kingfisher::random::Random::RandUInt32(0);
  EXPECT_TRUE(n == 0);
  n = kingfisher::random::Random::RandUInt32(1);
  EXPECT_TRUE(n == 0);

  int count = 20;
  std::vector<uint32_t> values(count);
  std::vector<std::thread> ths;
  for (int i = 0; i < count; ++i) {
    ths.push_back(std::move(std::thread([i, &values] {
      values[i] = kingfisher::random::Random::RandUInt32(100, 1000);
    })));
  }

  for (auto&& i : ths) {
    i.join();
  }

  std::sort(values.begin(), values.end());
  for (int i = 0; i < count - 1; ++i) {
    std::cout << values[i] << std::endl;
    EXPECT_LE(values[i], values[i + 1]);
    EXPECT_LE(values[i], 1000);
  }
}
