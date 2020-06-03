#include "file.h"
#include <sys/stat.h>
#include <unistd.h>
#include <cassert>
#include <iostream>
#include "core/likely.h"
#include "core/scope_guard.h"

namespace kingfisher {
namespace file {

File::File() noexcept : fd_(-1), fp_(nullptr), owns_fd_(false) {}

File::File(int fd, bool owns_fd /*= false*/) noexcept
    : fd_(fd), owns_fd_(owns_fd) {
  assert(fd_ > 0);
  fp_ = fdopen(fd_, "r");
}

#if 0
File::File(const std::string &file_name, int flags /* = O_RDONLY*/,
           unsigned int mode /* = 0666*/)
    : fd_(::open(file_name.c_str(), flags, mode)), owns_fd_(false) {
  if (fd_ == -1) {
    throw std::logic_error("open file failed");
  }

  owns_fd_ = true;
}
#endif

File::File(File&& other) : fd_(other.fd_), owns_fd_(other.owns_fd_) {}

size_t File::GetFileSize() const {
  if (!Valid()) {
    return 0;
  }

  struct stat statbuf;
  return (0 != ::fstat(fd_, &statbuf)) ? 0 : statbuf.st_size;
}

size_t File::GetPositon() const {
  if (!Valid()) {
    return 0;
  }

  return static_cast<size_t>(::lseek(fd_, 0, SEEK_CUR));
}

void File::Swap(File& other) {
  std::swap(fd_, other.fd_);
  std::swap(owns_fd_, other.owns_fd_);
  std::swap(fp_, other.fp_);
  std::swap(filename_, other.filename_);
}

/*static*/ File File::temporary() {
  using namespace kingfisher::core;
  FILE* tmp_file = std::tmpfile();
  SCOPE_EXIT {
    if (nullptr != tmp_file) {
      fclose(tmp_file);
    }
  };

  int fd = ::dup(::fileno(tmp_file));
  if (LIKELY(-1 != fd)) {
    return File(fd, true);
  }

  return File();
}

File File::Dup() const {
  if (-1 != fd_) {
    int fd = ::dup(fd_);
    if (LIKELY(-1 != fd)) {
      return File(fd, true);
    }
  }

  return File();
}

int File::Release() {
  int released = fd_;
  fd_ = -1;
  owns_fd_ = false;
  return released;
}

bool File::Close() {
  std::cout << "close fd_, filename: " << filename_ << std::endl;
  int ret = owns_fd_ ? ::close(fd_) : 0;
  if (fp_) {
    std::cout << "close fp_, filename: " << filename_ << std::endl;
    ::fclose(fp_);
    fp_ = nullptr;
  }
  Release();
  return (0 == ret);
}

}  // namespace file
}  // namespace kingfisher
