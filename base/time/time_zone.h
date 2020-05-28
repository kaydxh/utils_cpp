//
// Created by kayxhding on 2020-05-26 16:11:14
//

#ifndef KINGFISHER_BASE_TIME_TIMEZONE_H_
#define KINGFISHER_BASE_TIME_TIMEZONE_H_

#include <ctime>
#include <memory>

namespace kingfisher {
namespace time {

class TimeZone {
 public:
  explicit TimeZone(const char *zone_file);
  TimeZone(int east_of_utc, const char *tzone);  // a fixed timezone
  TimeZone() {}                                  // an invalid timezone
  ~TimeZone() {}

  bool Valid() const {
    // 'explicit operator bool() const' in C++11
    return static_cast<bool>(data_);
  }

  struct tm ToLocalTime(std::time_t seconds_since_epoch) const;

  time_t FromLocalTime(const struct tm &t) const;

  // gmtime(3)
  static struct tm ToUtcTime(std::time_t seconds_since_epoch,
                             bool yday = false);

  // timegm(3)
  static std::time_t FromUtcTime(const struct tm &t);

  // year in [1900..2500], month in [1..12], day in [1..31]
  static std::time_t FromUtcTime(int year, int month, int day, int hour,
                                 int minute, int seconds);

  struct Data;

 private:
  std::shared_ptr<Data> data_;
};
}  // namespace time
}  // namespace kingfisher

#endif
