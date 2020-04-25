#ifndef KINNGFISHER_CORE_NONCOPYABLE_HPP
#define KINNGFISHER_CORE_NONCOPYABLE_HPP

namespace kingfisher {

//  Private copy constructor and copy assignment ensure classes derived from
//  class noncopyable cannot be copied.

namespace noncopyable_  // protection from unintended ADL
{
class noncopyable {
 protected:
  noncopyable() = default;
  ~noncopyable() = default;

  noncopyable(const noncopyable&) = delete;
  noncopyable& operator=(const noncopyable&) = delete;
};
}  // namespace noncopyable_

typedef noncopyable_::noncopyable noncopyable;

}  // namespace kingfisher

#endif  //  KINNGFISHER_CORE_NONCOPYABLE_HPP
