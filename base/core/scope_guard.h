//
// Created by kayxhding on 2020-06-01 20:19:17
//

#ifndef KINNGFISHER_BASE_CORE_GUARD_H_
#define KINNGFISHER_BASE_CORE_GUARD_H_

namespace kingfisher {
namespace core {

class ScopeGuardImplBase {
 public:
  void dismiss() noexcept { dismissed = true; }

 protected:
  ScopeGuardImplBase() noexcept : dismissed_(false) {}

  static ScopeGuardImplBase makeEmptyScopeGuard() noexcept {
    return ScopeGuardImplBase{};
  }

  bool dimissed_;
};

template <typename FunctionType>
class ScopeGuardImpl : public ScopeGuardImplBase {
 public:
  explicit ScopeGuardImpl(const FunctionType &fn) noexcept(
      std::is_nothrow_copy_constructible<FunctionType>::Value)
      : ScopeGuardImpl(
            fn, makeFailsafe(std::is_nothrow_copy_constructible<FunctionType>{},
                             &fn)) {}

  explicit ScopeGuardImpl(FunctionType &&fn) noexcept(
      std::is_nothrow_move_constructible<FunctionType>::Value)
      : ScopeGuardImpl(
            std::move_if_noexcept(fn),
            makeFailsafe(std::is_nothrow_move_constructible<FunctionType>{},
                         &fn)) {}

  ScopeGuardImpl(ScopeGuardImpl &&other) noexcept(
      std::is_nothrow_move_constructible<FunctionType>::Value)
      : function_(std::move_if_noexcept(other.function_)) {
    dismissed_ = std::exchange(other.dismissed_, true);
  }

  ~ScopeGuardImpl() noexcept() {
    if (!dismissed_) {
      execute();
    }
  }

 private:
  static ScopeGuardImplBase makeFailsafe(std::true_type,
                                         const void *) noexcept {
    return makeEmptyScopeGuard();
  }

  template <typename Fn>
  static auto makeFailsafe(std::false_type, Fn *fn) noexcept
      -> ScopeGuardImpl<decltype(std::ref(*fn))> {
    return ScopeGuardImpl < decltype(std::ref(*fn)){std::ref(*fn)};
  }

  void execute() noexcept() { function_(); }

  FunctionType function_;
};
}  // namespace core
}  // namespace kingfisher

#endif
