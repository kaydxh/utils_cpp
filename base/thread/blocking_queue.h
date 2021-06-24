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

namespace kingfisher {
namespace thread {

template <typename T>
class BlockingQueue {
  using LockGuard = std::lock_guard<std::mutex>;

 public:
  BlockingQueue(int capacity = -1)
      : capacity_(capacity),
        mutex_(),
        notEmptyCond_(),
        notFullCond_(),
        queue_() {}

  ~BlockingQueue() {}

  void Put(const T &task) {
    std::unique_lock<std::mutex> lock(mutex_);
    while (full()) {
      notFullCond_.wait(lock, [this] { return !full(); });
    }
    queue_.push_back(task);
    notEmptyCond_.notify_one();
  }

  void Put(T &&task) {
    std::unique_lock<std::mutex> lock(mutex_);
    while (full()) {
      notFullCond_.wait(lock, [this] { return !full(); });
    }
    queue_.push_back(std::move(task));
    notEmptyCond_.notify_one();
  }

  void PutWait(int &return_, const T &task, int32_t timeout) {
    std::unique_lock<std::mutex> lock(mutex_);
    std::chrono::milliseconds timout(timeout);
    while (full()) {
      bool ret =
          notFullCond_.wait_for(lock, timout, [this] { return !full(); });
      if (!ret) {
        return_ = -1;
        return;
      }
    }
    queue_.push_back(task);
    notEmptyCond_.notify_one();
    return_ = 0;
    return;
  }

  T Take() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (empty()) {
      notEmptyCond_.wait(lock, [this] { return !empty(); });
    }
    assert(!empty());
    T front(std::move(queue_.front()));
    queue_.pop_front();

    return front;
  }

  T TakeWait(int &return_, int32_t timeout) {
    std::unique_lock<std::mutex> lock(mutex_);
    std::chrono::milliseconds timout(timeout);
    while (empty()) {
      bool ret =
          notEmptyCond_.wait_for(lock, timout, [this] { return !empty(); });
      if (!ret) {
        return_ = -1;
        return T();
      }
    }
    assert(!empty());
    T front(std::move(queue_.front()));
    queue_.pop_front();
    return_ = 0;
    return front;
  }

  size_t Size() const {
    LockGuard lock(mutex_);
    return size();
  }

  bool Empty() const {
    LockGuard lock(mutex_);
    return empty();
  }

  bool Full() const {
    LockGuard lock(mutex_);
    return full();
  }

  size_t size() const { return queue_.size(); }

  bool empty() const { return queue_.empty(); }

  bool full() const {
    if (capacity_ < 0) {
      return false;
    }
    return queue_.size() >= capacity_;
  }

 private:
  int capacity_;
  mutable std::mutex mutex_;
  std::condition_variable notEmptyCond_;
  std::condition_variable notFullCond_;
  std::deque<T> queue_;
};

}  // namespace thread
}  // namespace kingfisher

#endif  // BASE_THREAD_BLOCKING_QUEUE_H_
