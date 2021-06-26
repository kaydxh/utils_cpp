
#include "time/timer.h"

namespace kingfisher {
namespace time {

// implementation
// TimerEventBase
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

// TimerWheel
void TimerWheel::Start() {
  thread_pool_.start();
  thread_pool_.AddTask(&TimerWheel::runLoop, this);
}

void TimerWheel::runLoop() {
  while (running_) {
    if (!process_current_slot()) {
    }
    msleep(1);
  }
}

void TimerWheel::Join() { thread_pool_.join(); }

bool TimerWheel::process_current_slot() {
  auto slot = &slots_[cur_slot_index_];
  while (slot->events()) {
    // while ((&slots_[cur_slot_index_])->events()) {
    if (slot->events()->rotation_at_ > 0) {
      --slot->events()->rotation_at_;
      std::cout << "process rotation: " << slot->events()->rotation_at_
                << std::endl;
    } else {
      auto event = slot->popEvent();
      event->run();
      now_ = getJiffies();
      if (event->repeated_times_ == 1) {
        ;  // do nothing

      } else if (event->repeated_times_ > 1) {
        Schedule(event, event->interval_, --event->repeated_times_);

      } else {  // (event->repeated_times_ <= 0)
        Schedule(event, event->interval_, -1);
      }
    }
  }

  cur_slot_index_ = (cur_slot_index_ + 1) % NUM_SLOTS;

  return true;
}

void TimerWheel::Stop() {
  running_ = false;
  thread_pool_.stop();
}

void TimerWheel::Schedule(TimerEventBase* event, Tick interval,
                          int32_t repeated_times) {
  auto tm = getJiffies() - now_ + interval;
  int rotation = tm / NUM_SLOTS;
  size_t slot_index = (tm + cur_slot_index_) % NUM_SLOTS;
  event->set_rotation_at(rotation);
  event->interval_ = interval;
  event->repeated_times_ = repeated_times;
  //  std::cout << "rotation: " << rotation << ", slot_index: " << slot_index
  //           << std::endl;
  auto slot = &slots_[slot_index];

  event->relink(slot);
}

uint64_t getJiffies() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (ts.tv_sec * 1000 + ts.tv_nsec / 1e6);
}

void msleep(uint32_t msecs) {
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = msecs * 1000;
  select(0, nullptr, nullptr, nullptr, &tv);
  //  std::cout << "msleep" << msecs << std::endl;
}

}  // namespace time
}  // namespace kingfisher
