//
// Created by kayxhding on 2020-04-28 18:57:49
//

#ifndef BASE_THREAD_POOL_THREAD_H_
#define BASE_THREAD_POOL_THREAD_H_

#include <condition_variable>
#include <deque>
#include <future>
#include <mutex>
#include <vector>
#include "thread.h"

namespace kingfisher {
namespace thread {

class ThreadPool : noncopyable {
 public:
  using Task = std::function<void()>;

  explicit ThreadPool(int threads_number = 4);
  ~ThreadPool();

  void start();

  template <typename F, typename... Args>
  auto AddTask(F&& f, Args&&... args)
      -> std::future<typename std::result_of<F(Args...)>::type>;
  Task take();
  void loopInThread();

 private:
  mutable std::mutex mutex_;
  std::condition_variable condition_;
  std::deque<Task> tasks_;
  std::vector<std::unique_ptr<Thread>> threads_;

  int threads_number_;
  bool running_ = false;
};
}  // namespace thread
}  // namespace kingfisher

#endif
