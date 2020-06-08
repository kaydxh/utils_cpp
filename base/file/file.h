//
// Created by kayxhding on 2020-05-26 21:00:44
//
#ifndef KINGFISHER_BASE_TIME_ZONE_H_
#define KINGFISHER_BASE_TIME_ZONE_H_

#include <endian.h>
#include <stdint.h>
#include <stdio.h>
//#include <string.h>
#include <stdexcept>
#include "file_util.h"

#include <string>
#include "core/noncopyable.hpp"

namespace kingfisher {
namespace file {

class File : kingfisher::noncopyable {
 public:
  // Creates an empty File object, for late initialization
  File() noexcept;

  // File(const std::string& file_name, int flags = std::O_RDONLY,
  //     unsigned int mode = 0666);
  File(const char* filename)
      : filename_(filename) {  // fp_(::fopen(filename, "rb")) {
    fd_ = kingfisher::fileutil::Open(open, filename, flags, mode);
    if (valid()) {
      owns_fd_ = true;
    }
    //    fd_ = ::fileno(fp_);
  }

  explicit File(int fd, bool owns_fd = false) noexcept;

  File(File&& other);

  ~File() { Close(); }

  bool Valid() const { return -1 != fd_; }

  size_t GetFileSize() const;
  size_t GetPositon() const;
  bool SetPositon() const;

  size_t Read(void* buf, size_t size) {
    if (Valid()) {
      return 0;
    }

    return kingfisher::fileutil::ReadFull(fd_, buf, size);
  }

  std::string ReadBytes(int n) {
    char buf[n];
    ssize_t bytes = Read(buf, n);
    if (n != static_cast<int>(bytes)) {
      throw std::logic_error("no enough data to read");
    }

    return std::string(buf, n);
  }

  int32_t ReadInt32() {
    int32_t ret = 0;
    ssize_t bytes = Read(&ret, sizeof(int32_t));
    if (sizeof(int32_t) != static_cast<int>(bytes)) {
      throw std::logic_error("bad read int32_t data");
    }
    return static_cast<int32_t>(::be32toh(ret));
  }

  int32_t ReadUInt8() {
    uint8_t ret = 0;
    ssize_t bytes = Read(&ret, sizeof(uint8_t));
    if (sizeof(uint8_t) != static_cast<int>(bytes)) {
      throw std::logic_error("bad read uint8_t data");
    }

    return ret;
  }
#if 0
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
      throw std::logic_error("bad read int32_t data");
    }

    return static_cast<int32_t>(::be32toh(ret));
  }

  int32_t ReadUInt8() {
    uint8_t ret = 0;
    ssize_t bytes = ::fread(&ret, 1, sizeof(uint8_t), fp_);
    if (sizeof(uint8_t) != static_cast<int>(bytes)) {
      throw std::logic_error("bad read uint8_t data");
    }

    return ret;
  }
#endif
  int GetFd() const { return fd_; }
  // FILE* GetFile() const { return fp_; }

  void Swap(File& other);

  File Dup() const;

  static File temporary();

  int Release();

  bool Close();

 private:
  std::string filename_;
  int fd_;
  // FILE* fp_;
  bool owns_fd_;
};
}  // namespace file
}  // namespace kingfisher

#endif
