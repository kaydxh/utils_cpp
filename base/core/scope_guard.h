//
// Created by kayxhding on 2020-06-01 20:19:17
//

#ifndef KINNGFISHER_BASE_CORE_SCOPE_GUARD_H_
#define KINNGFISHER_BASE_CORE_SCOPE_GUARD_H_

#include <functional>
#include <type_traits>
#include <utility>

namespace kingfisher {
namespace core {

class ScopeGuardImplBase {
 public:
  void dismiss() noexcept { dismissed_ = true; }

 protected:
  ScopeGuardImplBase() noexcept : dismissed_(false) {}

#if 0
  static ScopeGuardImplBase makeEmptyScopeGuard() noexcept {
    return ScopeGuardImplBase{};
  }
#endif

  bool dismissed_;
};

template <typename FunctionType>
class ScopeGuardImpl : public ScopeGuardImplBase {
 public:
  explicit ScopeGuardImpl(const FunctionType &fn) : function_(fn) {}

#if 0
  explicit ScopeGuardImpl(const FunctionType &fn) noexcept(
      std::is_nothrow_copy_constructible<FunctionType>::value)
      : ScopeGuardImpl(
            fn, makeFailsafe(std::is_nothrow_copy_constructible<FunctionType>{},
                             &fn)) {}
#endif

  explicit ScopeGuardImpl(FunctionType &&fn) : function_(std::move(fn)) {}
#if 0
  explicit ScopeGuardImpl(FunctionType &&fn) noexcept(
      std::is_nothrow_move_constructible<FunctionType>::value)
      : ScopeGuardImpl(
            std::move_if_noexcept(fn),
            makeFailsafe(std::is_nothrow_move_constructible<FunctionType>{},
                         &fn)) {}
#endif

  ScopeGuardImpl(ScopeGuardImpl &&other)
      : ScopeGuardImplBase(std::move(other)),
        function_(std::move(other.function_)) {
    other.dismissed_ = true;
  }
#if 0
  ScopeGuardImpl(ScopeGuardImpl &&other) noexcept(
      std::is_nothrow_move_constructible<FunctionType>::Value)
      : function_(std::move_if_noexcept(other.function_)) {
    dismissed_ = std::exchange(other.dismissed_, true);
  }
#endif

  ~ScopeGuardImpl() {
    if (!dismissed_) {
      function_();
    }
  }

 private:
#if 0
  static ScopeGuardImplBase makeFailsafe(std::true_type,
                                         const void *) noexcept {
    return makeEmptyScopeGuard();
  }

  template <typename Fn>
  static auto makeFailsafe(std::false_type, Fn *fn) noexcept
      -> ScopeGuardImpl<decltype(std::ref(*fn))> {
    return ScopeGuardImpl<decltype(std::ref(*fn))>{std::ref(*fn)};
  }
#endif

  void *operator new(size_t) = delete;
  FunctionType function_;
};

enum class ScopeGuardOnExit {};

template <typename FunctionType>
ScopeGuardImpl<typename std::decay<FunctionType>::type> operator+(
    ScopeGuardOnExit, FunctionType &&fn) {
  return ScopeGuardImpl<typename std::decay<FunctionType>::type>(
      std::forward<FunctionType>(fn));
}

#if 1
#define SCOPE_EXIT \
  auto KF_ANONYMOUS_VARIABLES = ScopeGuardOnExit() + [&]() noexcept
#endif

}  // namespace core
}  // namespace kingfisher

#if 0
#define SCOPE_EXIT              \
  auto KF_ANONYMOUS_VARIABLES = \
      kingfisher::core::ScopeGuardOnExit() + [&]() noexcept
#endif

#endif
