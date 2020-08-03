#include <gtest/gtest.h>
#include <string>
#include "lru/lru_cache.hpp"

using namespace kingfisher;
using namespace kingfisher::lru;

class test_LruCache : public testing::Test {
 public:
  test_LruCache() {}
  ~test_LruCache() {}

  virtual void SetUp(void) {}

  virtual void TearDown(void) {}
};

TEST_F(test_LruCache, All) {
  LruCache<std::string, size_t> lru_cache(3);
  lru_cache.Put("a", 1);
  lru_cache.Put("b", 2);
  lru_cache.Put("c", 3);

  // order: c, b, a
  size_t value = 0;
  lru_cache.Get("a", value);
  EXPECT_EQ(value, 1);
  // order: a, c, b

  lru_cache.Put("d", 4);
  // order: d, a, c
  EXPECT_FALSE(lru_cache.Get("b", value));
}
