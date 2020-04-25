//
// Created by kayxhding on 2020-04-25 18:45:24
//
#include <gtest/gtest.h>
#include <future>
#include <iostream>
#include "thread/blocking_queue.h"

class test_BlockingQueue : public testing::Test {
 public:
  test_BlockingQueue() {}
  ~test_BlockingQueue() {}

  virtual void SetUp(void) {}

  virtual void TearDown(void) {}
};

TEST_F(test_BlockingQueue, Integer) {
  kingfisher::thread::BlockingQueue<int> q;
  auto t1 = std::async(std::launch::async, [&q]() {
    for (int i = 0; i < 10; ++i) {
      q.Put(i);
    }
  });

  auto t2 = std::async(std::launch::async, [&q]() {
    while (q.Size()) {
      ASSERT_TRUE(q.Take() < 10);
    }
  });

  auto t3 = std::async(std::launch::async, [&q]() {
    while (q.Size()) {
      ASSERT_TRUE(q.Take() < 10);
    }
  });

  t1.wait();
  t2.wait();
  t3.wait();
}
