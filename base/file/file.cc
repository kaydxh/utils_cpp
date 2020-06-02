#include "file.h"

namespace kingfisher {
namespace file {

File::File noexcept : fd_(-1), own_fd(false) {}

File::File(int fd, bool owns_fd = false) noexcept : fd_(fd), owns_fd_(owns_fd) {
  assert(fd_ > 0);
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

/*static*/ File File::temporary() { FILE *tmp }
}  // namespace file
}  // namespace kingfisher
