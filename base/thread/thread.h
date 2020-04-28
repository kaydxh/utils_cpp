//
// Created by kayxhding on 2020-04-28 15:49:03
//

#ifndef BASE_THREAD_THREAD_H_
#define BASE_THREAD_THREAD_H_

#include <functional>
#include <thread>
#include "core/noncopyable.hpp"

namespace kingfisher {
namespace thread {

class Thread : noncopyable {
 public:
  using ThreadFunc = std::function<void()>;

  explicit Thread(ThreadFunc);
  ~Thread();

 private:
  void run();

 private:
  std::thread thread_;
  ThreadFunc func_;
};
}  // namespace thread
}  // namespace kingfisher

#endif
