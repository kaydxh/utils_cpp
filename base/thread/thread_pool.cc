//
// Created by kayxhding on 2020-04-28 18:57:54
//

#include "thread_pool.h"
#include <iostream>

namespace kingfisher {
namespace thread {

ThreadPool::ThreadPool(int threads_number)
    : mutex_(),
      condition_(),
      tasks_(),
      tasks_remaining_(0),
      threads_number_(threads_number) {}

ThreadPool::~ThreadPool() {}

void ThreadPool::start() {
  running_ = true;
  threads_.reserve(threads_number_);
  for (int i = 0; i < threads_number_; ++i) {
    threads_.emplace_back(
        new Thread(std::bind(&ThreadPool::loopInThread, this)));
  }
}

ThreadPool::Task ThreadPool::take() {
  std::unique_lock<std::mutex> lock(mutex_);
  condition_.wait(lock, [this] { return !running_ || !tasks_.empty(); });

  Task task;
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
      std::cout << "--" << tasks_remaining_ << std::endl;
      if (0 == --tasks_remaining_) {
        std::unique_lock<std::mutex> lock(mutex_);
        std::cout << "-------" << tasks_remaining_ << std::endl;
        condition_.notify_all();
      }
    }
  }
}

void ThreadPool::stop() {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    running_ = false;
    condition_.notify_all();
  }

  std::cout << "start join" << std::endl;
  for (auto &thr : threads_) {
    thr->join();
  }
  std::cout << "end finish" << std::endl;
}

void ThreadPool::join() {
  std::unique_lock<std::mutex> lock(mutex_);
  while (tasks_remaining_) {
    std::cout << "AA" << std::endl;
    condition_.wait(lock);
    std::cout << "BB" << std::endl;
  }
}
}  // namespace thread
}  // namespace kingfisher
