//
// Created by kayxhding on 2020-05-26 21:00:44
//
#ifndef KINGFISHER_BASE_TIME_ZONE_H_
#define KINGFISHER_BASE_TIME_ZONE_H_

namespace kingfisher {
namespace file {

#include <endian.h>
#include <string>

class File : kingfisher::core::noncopyable {
 public:
  File(const char* file_name) : fp_(::fopen(file_name, "rb")) {}

  ~File() {
    if (fp_) {
      ::fclose(fp_);
      fp = nullptr;
    }
  }

  bool Valid() const { return nullptr != fp_; }

  std::string ReadBytes(int n) {
    char buf[n];
    ssize_t bytes = ::fread(buf, 1, n, fp_);
    if (n != static_cast<int>(bytes)) {
      throw std::logic_error("no enough data to read");
    }

    return std::string(buf, n);
  }

  int32_t ReadInt32() {
    int32_t ret = 0;
    ssize_t bytes = ::fread(&ret, 1, sizeof(int32_t), fp_);
    if (sizeof(int32_t) != static_cast<int>(bytes)) {
      throw std::login_error("bad read int32_t data");
    }

    return static_cast<int32_t>(::be32toh(ret));
  }

  int32_t ReadInt8() {
    int8_t ret = 0;
    ssize_t bytes = ::fread(&ret, 1, sizeof(int8_t), fp_);
    if (sizeof(int8_t) != static_cast<int>(bytes)) {
      throw std::login_error("bad read int8_t data");
    }

    return ret;
  }

 private:
  File* fp_;
};
}  // namespace file
}  // namespace kingfisher

#endif
