//
// Created by kayxhding on 2020-05-21 15:34:47
//

#ifndef KINGFISHER_BASE_TIME_TIMESTAMP_H_
#define KINGFISHER_BASE_TIME_TIMESTAMP_H_

#include <inttypes.h>
#include <sys/time.h>

#include <string>

namespace kingfisher {
namespace time {

class Timestamp {
 public:
  Timestamp();
  ~Timestamp();
  explicit Timestamp(const int64_t micro_seconds_since_epoch_);

  Timestamp(const Timestamp &rhs);
  Timestamp &operator=(const Timestamp &rhs);

  void Swap(Timestamp &that) {
    std::swap(micro_seconds_since_epoch_, that.micro_seconds_since_epoch_);
  }

  std::string ToSecDotMicroString() const;
  std::string ToString() const;

  std::string ToFormattedString(bool show_microseconds = true) const;

  bool Valid() const { return micro_seconds_since_epoch_ > 0; };

  int64_t MicroSecondsSinceEpoch() const { return micro_seconds_since_epoch_; }

  time_t SecondsSinceEpoch() const {
    return static_cast<time_t>(micro_seconds_since_epoch_ /
                               kMicroSecondsPerSecond);
  }

  static Timestamp Now();

  static Timestamp Invalid();

  static const int kMicroSecondsPerSecond = 1000 * 1000;
  static const int kMicroSecondsPerMilliSecond = 1000;

 private:
  int64_t micro_seconds_since_epoch_;
};

inline bool operator<(const Timestamp &lhs, const Timestamp &rhs) {
  return lhs.MicroSecondsSinceEpoch() < rhs.MicroSecondsSinceEpoch();
}

inline bool operator==(const Timestamp &lhs, const Timestamp &rhs) {
  return lhs.MicroSecondsSinceEpoch() == rhs.MicroSecondsSinceEpoch();
}

inline bool operator!=(const Timestamp &lhs, const Timestamp &rhs) {
  return !(lhs == rhs);
}

inline bool operator>(const Timestamp &lhs, const Timestamp &rhs) {
  return !(lhs < rhs) && (lhs != rhs);
}

inline double DiffTimerenceSecond(const Timestamp &high, const Timestamp &low) {
  int64_t diff = high.MicroSecondsSinceEpoch() - low.MicroSecondsSinceEpoch();
  return static_cast<double>(diff) / Timestamp::kMicroSecondsPerSecond;
}

inline double DiffTimerenceMilliSecond(const Timestamp &high,
                                       const Timestamp &low) {
  int64_t diff = high.MicroSecondsSinceEpoch() - low.MicroSecondsSinceEpoch();
  return static_cast<double>(diff) / Timestamp::kMicroSecondsPerMilliSecond;
}

inline Timestamp AddTime(const Timestamp &timestamp, double seconds) {
  int64_t delta =
      static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond);
  return Timestamp(timestamp.MicroSecondsSinceEpoch() + delta);
}

}  // namespace time
}  // namespace kingfisher

#endif
