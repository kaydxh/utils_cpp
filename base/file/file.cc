#include "file.h"
#include <unistd.h>
#include <cassert>
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

void File::Swap(File& other) {
  std::swap(fd_, other.fd_);
  std::swap(owns_fd_, other.owns_fd_);
  std::swap(fp_, other.fp_);
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
  int ret = owns_fd_ ? ::close(fd_) : 0;
  if (fp_) {
    ::fclose(fp_);
    fp_ = nullptr;
  }
  Release();
  return (0 == ret);
}

}  // namespace file
}  // namespace kingfisher
