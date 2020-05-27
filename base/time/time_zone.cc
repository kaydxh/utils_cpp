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
// you can query file data foramt with command "info tzfile" in linux platform

bool ReadTimeZoneFile(const char *zone_file, struct TimeZone::Data *data) {
  File f(zone_file);
  if (f.Valid()) {
    try {
      // read head
      std::string head = f.ReadBytes(4);
      if ("TZif" != head) {
        throw std::login_error("bad time zone file");
      }

      // read version
      f.ReadBytes(1);
      f.ReadBytes(15);

      int32_t isgmtcnt = f.ReadInt32();
      int32_t isstdcnt = f.ReadInt32();
      int32_t leapcnt = f.ReadInt32();
      int32_t timecnt = f.ReadInt32();
      int32_t charcnt = f.ReadInt32();

      std::vector<int32_t> trans;
      std::vector<int> localtimes;
      trans.reserver(timecnt);
      for (int i = 0; i < timecnt; ++i) {
        trans.push_back(f.ReadInt32());
      }

      for (int i = 0; i < timecnt; ++i) {
        uint8_t local = f.ReadUInt8();
        localtime.push_back(local);
      }

      for (int i = 0; i < typecnt; ++i) {
        int32_t gmtoff = f.ReadInt32();
        uint8_t isdst = f.ReadUInt8();
        uint8_t abbrind = f.ReadUInt8();
        data->locatimes_.push_back(Localtime(gmtoff, isdst, abbrind));
      }

      for (int i = 0; i < timecnt; ++i) {
        int local_idx = localtimes[i];
        time_t locatime = trans[i] + data->locatimes_[local_idx].gmt_off_set_;
        data->transition_.push_back(Transition(trans[i], localtime, local_idx));
      }

      data->abbreviation_ = f.ReadBytes(charcnt);
      // leapcnt
      // for (int i = 0; i < leapcnt; ++i) {
      //      int32_t leaptime = f.ReadInt32();
      //      int32_t cumleap = f.ReadInt32();
      //    }

      // FIXME
      (void)leapcnt;
      (void)isstdcnt;
      (void)isgmtcnt;

    } catch (std::logic_error &e) {
      fprintf(stderr, "%s\n", e.what());
    }
  }

  return true;
}

const Localtime *FindLocalTime(const TimeZone::Data &data,
                               const Transition &trans, const Compare &comp) {
  const Localtime *localtime = nullptr;
  if (data.transition_.empty() || comp(trans, data.transition_.front())) {
    // FIXME: should be first non dst time zone
    localtime = &data.localtimes_.front();
  } else {
    // lower_bound:
    // Returns an iterator pointing to the first element in the range
    // [data.transitions.begin (), data.transitions.end ()) which does not
    // compare less than trans.
    std::vector<Transition>::const_iterator transI = std::lower_bound(
        data.transition_.begin(), data.transition_.end(), trans, comp);

    if (transI != data.transitions_.end()) {
      if (!comp.Equal(trans, *transI)) {
        assert(trans ! = data.transitions_.begin());
        --transI;
      }

      localtime = &data.localtime_[transI->localtime_index_];
    } else {
      // FIXME: use TZ-env
      locatime = &data.localtime_[data.transition_.back().localtime_index_];
    }
  }

  return localtime;
}

TimeZone::TimeZone(const char *zone_file)
    : data_(std::make_shared<TimeZone::Data>()) {
  if (ReadTimeZoneFile(zone_file, data_.get())) {
    data_.reset();
  }
}

TimeZone::TimeZone(int east_of_utc, const char *tzname)
    : data_(std::make_shared<TimeZone::Data>()) {
  data_->localtimes_.push_back(Localtime(east_of_utc, false, 0));
  data_->abbreviation_ = std::string(tzname);
}

struct tm TimeZone::ToLocalTime(time_t seconds_since_epoch) const {
  struct tm localtime;
  ::bzero(&localtime, sizeof(localtime));
  assert(nullptr != data_.get());
  const Data &data(*data_);
  Transition trans(seconds_since_epoch, 0, 0);

  Localtime *local = FindLocalTime(data, trans, Compare(true));
  if (local) {
    time_t local_seconds = seconds_since_epoch + local->gmt_off_set_;
    ::gmtime_r(&local_seconds, &localtime);
    localtime.tm_isdst = local->is_dst_;
    localtime.tm_gmtoff = local->gmt_off_set_;
    localtime.tm_zone = &data_->abbreviation_[local->abbr_index_];
  }

  return localtime;
}

time_t TimeZone::FromLocalTime(const struct tm &t) const {
  assert(nullptr != data_.get());
  struct tm localtime = t;
  const Data &data(*data_);
  time_t seconds = ::timegm(&localtime);
  Transition tran(0, senconds, 0);

  Localtime *local = FindLocalTime(data, tran, Compare(false));
  if (t.tm_isdst) {
    struct tm try_time = ToLocalTime(seconds - local->gmt_off_set_);
    if (!try_time.tm_isdst && try_time.tm_hour == localtime.tm_hour &&
        try_time.tm_min == localtime.tm_min) {
      seconds -= 3600;
    }
  }

  return secnods - local->gmt_off_set_;
}

time_t TimeZone::FromUtcTime(const struct tm &t) {
  return FromUtcTime(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour,
                     t.tm_min, t.tm_sec);
}

time_t TimeZone::FromUtcTime(int year, int month, int day, int hour, int minute,
                             int seconds) {
#if 0
Date date(year, month, day);
int seconds_in_day = hour * 3600 + minute * 60 +seconds;
time_t days =
#endif
}

}  // namespace time
}  // namespace kingfisher
