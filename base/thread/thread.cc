#include "thread.h"
#include <exception>
#include <iostream>

namespace kingfisher {
namespace thread {

Thread::Thread(ThreadFunc func)
    : thread_(std::bind(&Thread::run, this)), func_(func) {}

Thread::~Thread() {
  if (thread_.joinable()) {
    thread_.join();
  }
}

void Thread::run() {
  if (nullptr == func_) {
    return;
  }

  try {
    func_();
  } catch (std::exception &ex) {
    std::cerr << "unhandle std::exception: " << ex.what() << std::endl;
  } catch (...) {
    std::cerr << "unhandle unknown exception" << std::endl;
  }

  std::cout << "finish task" << std::endl;
  return;
}

void Thread::join() {
  if (thread_.joinable()) {
    thread_.join();
  }
}
}  // namespace thread
}  // namespace kingfisher
