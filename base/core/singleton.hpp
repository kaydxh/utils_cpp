//
// Created by kayxhding on 2020-04-27 16:48:20
//

#ifndef KINNGFISHER_CORE_SINGLETON_HPP
#define KINNGFISHER_CORE_SINGLETON_HPP

#include <mutex>
#include "noncopyable.hpp"

namespace kingfisher {
namespace core {

#define SAFE_DELETE(x) \
  do {                 \
    if (x) delete x;   \
    x = nullptr;       \
  } while (0)

template <typename T>
struct has_no_destroy {
  template <typename C>
  static char test(decltype(&C::no_destroy));

  template <typename C>
  static int32_t test(...);
  const static bool value = sizeof(test<T>(0)) == 1;
};

template <typename T>
class Singleton : kingfisher::noncopyable {
 public:
  static T& Instance() {
    try {
      std::call_once(once_, [&]() { Singleton::init(); });
      return *value_;

    } catch (...) {
      exit(-1);
    }
  }

 protected:
  Singleton() {}
  ~Singleton() {}

 private:
  static void init() {
    value_ = new T();
    if (has_no_destroy<T>::value) {
      ::atexit(destory);
    }
  }

  static void destory() {
    using T_must_be_complete_type = char[sizeof(T) == 0 ? -1 : 1];
    T_must_be_complete_type dumpy;
    (void)dumpy;

    SAFE_DELETE(value_);
  }

 private:
  static std::once_flag once_;
  static T* value_;
};

template <typename T>
std::once_flag Singleton<T>::once_;

template <typename T>
T* Singleton<T>::value_ = nullptr;

}  // namespace core
}  // namespace kingfisher

#endif
