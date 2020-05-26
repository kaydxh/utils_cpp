#include "time_zone.h"

namespace kingfisher {
namespace time {

struct Transition {
  Transition(time_t gmt, time_t local, int index)
      : gmt_time_(gmt), localtime_(local), localtime_index_(index) {}

  time_t gmt_time_;
  time_t localtime_;
  int localtime_index_;
};

struct Compare {
  Compare(bool gmt) : compare_gmt_(gmt) {}
  ~Compare() {}

  bool operator()(const Transition &lhs, const Transition &rhs) const {
    return compare_gmt_ ? (lhs.gmt_time_ < rhs.gmt_time_)
                        : (lhs.localtime_ < rhs.localtime_);
  }

  bool Equal(const Transition &lhs, const Transition &rhs) const {
    return compare_gmt_ ? (lhs.gmt_time_ == rhs.gmt_time_)
                        : (lhs.localtime_ == rhs.localtime_);
  }

  bool compare_gmt_;
};

struct Localtime {
  Localtime(time_t off_set, bool dst, int arrb)
      : gmt_off_set_(off_set), is_dst_(dst), arrb_index_(arrb) {}

  time_t gmt_off_set_;
  bool is_dst_;
  int arrb_index_;
};

inline void FillHMS(unsigned int seconds, struct tm *utc) {
  utc->tm_sec = seconds % 60;
  unsigned int minutes = static_cast<unsigned int>(seconds / 60);
  utc->tm_min = minutes % 60;
  utc->tm_hour = static_cast<int>(minutes / 60);
}

const int kSecondsPerDay = 24 * 60 * 60;

struct TimeZone::Data {
  std::vector<Transition> transitions_;
  std::vector<Localtime> localtimes_;
  std::vector<std::string> names_;
  std::string abbreviation_;
};

// zone file in the directory of /usr/share/zoneinfo/ etc. localtime
// localtime -> /etc/localtime

bool ReadTimeZoneFile(const char *zone_file, struct TimeZone::Data *data) {}

}  // namespace time
}  // namespace kingfisher
