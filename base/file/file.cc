#include "file.h"
#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cassert>
#include <iostream>
#include "core/likely.h"
#include "core/scope_guard.h"

namespace kingfisher {
namespace file {

File::File() noexcept : fd_(-1), owns_fd_(false) {}

File::File(int fd, bool owns_fd /*= false*/) noexcept
    : fd_(fd), owns_fd_(owns_fd) {
  assert(fd_ > 0);

  char buf[256] = {'\0'};
  char file_name_buf[256] = {'\0'};
  snprintf(buf, sizeof(buf), "/proc/self/fd/%d", fd_);
  if (readlink(buf, file_name_buf, 255) < 0) {
    std::cerr << "readlink error" << std::endl;
    return;
  }

  filename_ = std::string(file_name_buf);
}

File::File(const char* filename, int flags /*= O_RDWR | O_LARGEFILE | O_CREAT*/,
           mode_t mode /*= 0666*/)
    : filename_(filename) {
  if (fd_) {
    Close();
  }

  fd_ = kingfisher::fileutil::Open(filename, flags, mode);
  if (Valid()) {
    owns_fd_ = true;
  }
}

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
  // std::swap(fp_, other.fp_);
  std::swap(filename_, other.filename_);
}

/*static*/ File File::Temporary() {
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

void File::Lock() { doLock(LOCK_EX); }

int File::Release() {
  int released = fd_;
  fd_ = -1;
  owns_fd_ = false;
  return released;
}

bool File::DeleteFile() {
  return kingfisher::fileutil::DeleteFile(filename_.c_str());
}

void File::Unlock() { kingfisher::fileutil::Flock(fd_, LOCK_UN); }

bool File::TryLock() { return doTryLock(LOCK_EX); }

bool File::TryLockShared() { return doTryLock(LOCK_SH); }

void File::UnlockShared() { Unlock(); }

void File::doLock(int op) { kingfisher::fileutil::Flock(fd_, op); }

bool File::doTryLock(int op) {
  int ret = kingfisher::fileutil::Flock(fd_, op | LOCK_NB);
  if (-1 == ret || EWOULDBLOCK == ret) {
    return false;
  }

  return true;
}

bool File::Close() {
  std::cout << "close fd_, filename: " << filename_ << std::endl;
  int ret = owns_fd_ ? ::close(fd_) : 0;
  Release();
  return (0 == ret);
}

}  // namespace file
}  // namespace kingfisher
