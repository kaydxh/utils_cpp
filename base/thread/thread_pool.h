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
      -> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();
    {
      std::unique_lock<std::mutex> lock(mutex_);
      tasks_.emplace_back([task]() { (*task)(); });
    }
    condition_.notify_one();

    return res;
  }

  Task take();
  void loopInThread();
  void stop();

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
