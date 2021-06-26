
#include "time/time_counter.h"

#include <cstdio>
#include <functional>

#include "time/timestamp.h"

namespace kingfisher {
namespace time {

TimeCounter::TimeCounter(bool effect) : effect_(effect) { Tick("start"); }

TimeCounter::~TimeCounter() {}

void TimeCounter::Tick(const std::string &msg) {
  if (effect_) {
    starts_.emplace_back(Timestamp::Now());
    messages_.emplace_back(msg);
  }
}

std::string TimeCounter::String() {
  if (!effect_) {
    return "";
  }

  auto f = [&](int idx, std::string msg, double cost,
               std::string at) -> std::string {
    char buf[1024];
    snprintf(buf, sizeof(buf), "#%d, msg: %s, cost: %fms, at %s\n", idx,
             msg.c_str(), cost, at.c_str());
    return std::string(buf);
  };

  return summary(f);
}

std::string TimeCounter::summary(
    std::function<std::string(int, std::string, double, std::string)> const
        &f) {
  if (!effect_) {
    return "";
  }

  if (messages_.size() < starts_.size()) {
    return "";
  }

  std::string result;
  for (std::vector<Timestamp>::size_type i = 1; i < starts_.size(); ++i) {
    result +=
        f(i, messages_[i], DiffTimerenceMilliSecond(starts_[i], starts_[i - 1]),
          starts_[i].ToFormattedString());
  }

  return result;
}

void TimeCounter::Reset() {
  starts_.clear();
  messages_.clear();
}

}  // namespace time
}  // namespace kingfisher

