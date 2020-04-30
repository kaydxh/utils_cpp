#include <gtest/gtest.h>
#include "thread/thread_pool.h"

using namespace kingfisher;
using namespace kingfisher::thread;

class test_ThreadPool : public testing::Test {
 public:
  test_ThreadPool() {}
  ~test_ThreadPool() {}

  virtual void SetUp(void) {}

  virtual void TearDown(void) {}
};

void taskAdd(int a, int b) {
  a += b;
  std::cout << "do worker task add result= " << a << std::endl;
}

class Worker {
 public:
  void task(int a, int b) {
    a += b;
    std::cout << "do worker task add result= " << a << std::endl;
    // return a;
  }
};

TEST(test_ThreadPool, theadpool) {
  ThreadPool pool(4);
  pool.start();
  pool.AddTask(taskAdd, 2, 3);

  std::function<void()> taskAdd_2 = std::bind(taskAdd, 29, 3);
  pool.AddTask(taskAdd_2);

  Worker w;
  pool.AddTask(&Worker::task, &w, 2, 3);
  pool.stop();
  // ThreadPool::Task func = std::move([]() { taskAdd(2, 3); });
  // pool.AddTask(func);

  //  auto result = pool.AddTask([](int answer) { return answer; }, 42);
  // std::cout << result.get() << std::endl;
}
