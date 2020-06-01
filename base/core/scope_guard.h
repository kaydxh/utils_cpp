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

template <typename FunctionType, bool InvokeNoexcept>
class ScopeGuardImpl : public ScopeGuardImplBase {};
}  // namespace core
}  // namespace kingfisher

#endif
