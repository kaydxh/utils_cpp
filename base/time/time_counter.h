
#ifndef KINGFISHER_BASE_TIME_TIMECOUNTER_H_
#define KINGFISHER_BASE_TIME_TIMECOUZONE_H_

#include <functional>
#include <string>
#include <vector>

#include "time/timestamp.h"

namespace kingfisher {
namespace time {

class Timestamp;

class TimeCounter {
 public:
  TimeCounter(bool effect = true);
  ~TimeCounter();

  void Tick(const std::string &msg);
  std::string String();
  void Reset();

 private:
  std::string summary(std::function<std::string(int, std::string, double,
                                                std::string)> const &f);

 private:
  // std::vector<uint64_t> starts_;
  std::vector<Timestamp> starts_;
  std::vector<std::string> messages_;
  bool effect_;
};

}  // namespace time
}  // namespace kingfisher

#endif

