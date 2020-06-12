//
// Created by kayxhding on 2020-06-11 17:40:58
//

#ifndef KINNGFISHER_BASE_CORE_TIMER_H_
#define KINNGFISHER_BASE_CORE_TIMER_H_

#include "core/disable.h"

namespace kingfisher {
namespace time {

class TimerWheelSlot;
class TimerWheel;

using Tick = uint64_t;

class TimerEventBase {
 public:
  TimerEventBase() {}
  virtual ~TimerEventBase() {}

  inline void cancel();

  bool action() const { return nullptr != slot_; }

 private:
  DISABLE_COPY_AND_ASSIGN(TimerEventBase);
  friend TimerWheelSlot;
  friend TimerWheel;
  virtual void run() = 0;
  virtual void relink(TimerWheelSlot* slot);

  void set_scheduled_at(Tick ts) { scheduled_at_ = ts; }

 private:
  TimerWheelSlot* slot_ = nullptr;
  TimerEventBase* prev_ = nullptr;
  TimerEventBase* next_ = nullptr;

  Tick scheduled_at_;
};

template <typename CBType>
class TimerEvent : public TimerEventBase {
 public:
  explicit TimerEvent<CBType>(const CBType& cb) : cb_(cb) {}

  void run() { cb_(); }

 private:
  DISABLE_COPY_AND_ASSIGN(TimerEvent);

 private:
  CBType cb_;
};

class TimerWheelSlot {
 public:
  TimerWheelSlot() {}
  ~TimerWheelSlot() {}

 private:
  DISABLE_COPY_AND_ASSIGN(TimerWheelSlot);
  friend TimerEventBase;
  friend TimerWheel;

  // return the fist event queued in slot
  const TimerEventBase* events() const { return events_; }

  // deque the first event from the slot, and return it
  TimerEventBase* popEvent() {
    auto event = events_;
    events_ = event->next_;
    if (events_) {
      events_->prev_ = nullptr;
    }
    event->next_ = nullptr;
    event->slot_ = nullptr;

    return event;
  }

 private:
  TimerEventBase* events_ = nullptr;
};

class TimerWheel {
 public:
  TimerWheel(Tick now = 0) : now_(now), ticks_pending_(0) {}
  ~TimerWheel() {}

  // a repeating timeout event that will fire every "interval" time
  void scheduleRepeating(TimerEventBase* event, Tick interval);

 private:
  bool process_current_slot(Tick now, size_t max_execute);

 private:
  static const int WIDTH_BITS = 8;
  static const int NUM_SLOTS = 1 << WIDTH_BITS;

  static const int MASK = (NUM_SLOTS - 1);
  Tick now_;
  Tick ticks_pending_;
  TimerWheelSlot slots_[NUM_SLOTS];
};

// implementation

void TimerEventBase::relink(TimerWheelSlot* new_slot) {
  if (new_slot == slot_) {
    return;
  }
  if (slot_) {
    auto prev = prev_;
    auto next = next_;
    if (next) {
      next->prev_ = prev;
    }

    if (prev) {
      prev->next_ = next;
    } else {
      // Must be at head of slot. Move the next item to the head
      slot_->events_ = next;
    }
  }

  {
    if (new_slot) {
      auto old = new_slot->events_;
      next_ = old;
      if (old) {
        old->prev_ = this;
      }
      new_slot->events_ = this;
    } else {
      next_ = nullptr;
    }

    // head of slot, set prev_ to nullptr
    prev_ = nullptr;
  }

  slot_ = new_slot;
}

void TimerEventBase::cancel() {
  if (!slot_) {
    return;
  }

  relink(nullptr);
}

bool TimerWheel::process_current_slot(Tick now, size_t max_execute) {
  size_t slot_index = now & MASK;
  auto slot = &slots_[slot_index];
  while (slot->events()) {
    auto event = slot->popEvent();
    event->run();
    if (!--max_execute) {
      return false;
    }
  }

  return true;
}

void TimerWheel::scheduleRepeating(TimerEventBase* event, Tick interval) {
  event->set_scheduled_at(now_ + interval);
  while (interval >= NUM_SLOTS) {
    interval = (interval + (now_ & MASK)) >> WIDTH_BITS;
  }

  size_t slot_index = (now_ + interval) & MASK;
  auto slot = &slots_[slot_index];
  event->relink(slot);
}
}  // namespace time
}  // namespace kingfisher
#endif
