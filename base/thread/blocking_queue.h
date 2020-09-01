//
// Created by kayxhding on 2020-04-25 17:38:04
//

#ifndef BASE_THREAD_BLOCKING_QUEUE_H_
#define BASE_THREAD_BLOCKING_QUEUE_H_

#include <assert.h>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include "core/noncopyable.hpp"

namespace kingfisher {
namespace thread {

template <typename T>
class BlockingQueue : noncopyable {
  using LockGuard = std::lock_guard<std::mutex>;

 public:
  BlockingQueue() : mutex_(), cond_(), queue_() {}

  ~BlockingQueue() {}

  void Put(const T &task) {
    LockGuard lock(mutex_);
    queue_.push_back(task);
    cond_.notify_one();
  }

  void Put(T &&task) {
    LockGuard lock(mutex_);
    queue_.push_back(std::move(task));
    cond_.notify_one();
  }

  T Take() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (queue_.empty()) {
      cond_.wait(lock, [this] { return !queue_.empty(); });
    }
    assert(!queue_.empty());
    T front(std::move(queue_.front()));
    queue_.pop_front();

    return front;
  }

  T TakeWait(int &return_, int32_t tm) {
    std::unique_lock<std::mutex> lock(mutex_);
    std::chrono::milliseconds timout(tm);
    while (queue_.empty()) {
      bool ret =
          cond_.wait_for(lock, timout, [this] { return !queue_.empty(); });
      if (!ret) {
        return_ = -1;
        return T();
      }
    }
    assert(!queue_.empty());
    T front(std::move(queue_.front()));
    queue_.pop_front();
    return_ = 0;
    return front;
  }

  size_t Size() const {
    LockGuard lock(mutex_);
    return queue_.size();
  }

  bool IsEmpty() const {
    LockGuard lock(mutex_);
    return queue_.empyt();
  }

 private:
  mutable std::mutex mutex_;
  std::condition_variable cond_;
  std::deque<T> queue_;
};

}  // namespace thread
}  // namespace kingfisher

#endif  // BASE_THREAD_BLOCKING_QUEUE_H_
