//
// Created by kayxhding on 2020-06-11 17:40:58
//

#ifndef KINNGFISHER_BASE_CORE_TIMER_H_
#define KINNGFISHER_BASE_CORE_TIMER_H_

#include "core/disable.h"

namespace kingfisher {
namespace time {

using Tick = uint64_t;

class TimerEventBase {
 public:
  TimerEventBase() {}
  virtual ~TimerEventBase() {}

  inline void cancel();

  bool action() const { return nullptr != slot_; }

 private:
  DISABLE_COPY_AND_ASSIGN(TimerEventBase)
  virtual void run() = 0;

 private:
  TimerWheelSlot* slot_ = nullptr;
  TimerWheelSlot* prev_ = nullptr;
  TimerWheelSlot* next_ = nullptr;
};

template <typename CBType>
class TimerEvent : public TimeEventBase {
 public:
  explicit TimerEvent<CBType>(const CBType& cb) : cb_(cb) {}

  void run() { cb(); }

 private:
  DISABLE_COPY_AND_ASSIGN(TimerEvent)

 private:
  CBType cb_;
};

class TimerWheelSlot {
 public:
  TimerWheelSlot() {}
  ~TimerWheelSlot() {}

 private:
  DISABLE_COPY_AND_ASSIGN(TimerWheelSlot)

  TimerEventBase* popEvent() {
    auto event = events_;
    events_ = events->next;
    if (events) {
      events_->prev = nullptr;
    }
    event->next = nullptr;
    event->slot_ = nullptr;

    return event;
  }

 private:
  TimerEventBase* events_ = nullptr;
};

class TimerWheel {
 public:
  TimerWheel() {}

 private:
  static const int WIDTH_BITS = 8;
  static const int NUM_LEVELS = (64 + WIDTH_BITS - 1) / WIDTH_BITS;
  static const int MAX_NUM_LEVELS = NUM_LEVELS - 1;
  static const int NUM_SLOTS = 1 << WIDTH_BITS;

  static const int MASK = (NUM_SLOTS - 1);
  Tick now_[NUM_LEVELS];
  Tick ticks_pending_;
  TimerWheelSlot slots_[]
};

}  // namespace time
}  // namespace kingfisher
#endif
