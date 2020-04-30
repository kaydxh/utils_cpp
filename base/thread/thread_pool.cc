//
// Created by kayxhding on 2020-04-28 18:57:54
//

#include "thread_pool.h"
#include <iostream>

namespace kingfisher {
namespace thread {

ThreadPool::ThreadPool(int threads_number)
    : mutex_(), condition_(), tasks_(), threads_number_(threads_number) {}

ThreadPool::~ThreadPool() {}

void ThreadPool::start() {
  running_ = true;
  threads_.reserve(threads_number_);
  for (int i = 0; i < threads_number_; ++i) {
    threads_.emplace_back(
        new Thread(std::bind(&ThreadPool::loopInThread, this)));
  }
}

template <typename F, typename... Args>
auto ThreadPool::AddTask(F &&f, Args &&... args)
    -> std::future<typename std::result_of<F(Args...)>::type> {
  using return_type = typename std::result_of<F(Args...)>::type;
  auto task = std::make_shared<std::packaged_task<return_type()> >(
      std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  std::future<return_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(mutex_);
    tasks_.emplace([task]() { (*task)(); });
  }
  condition_.notify_one();

  return res;
}

ThreadPool::Task ThreadPool::take() {
  std::unique_lock<std::mutex> lock(mutex_);
  condition_.wait(lock, [this] { return !running_ || !tasks_.empty(); });

  Task task;
  if (task) {
    std::cout << "task init" << std::endl;
  } else {
    std::cout << "task empty" << std::endl;
  }

  if (!running_) {
    return task;
  }
  if (!tasks_.empty()) {
    task = std::move(tasks_.front());
    tasks_.pop_front();
  }

  return task;
}

void ThreadPool::loopInThread() {
  while (running_) {
    Task task(take());
    if (task) {
      task();
    }
  }
}
}  // namespace thread
}  // namespace kingfisher
