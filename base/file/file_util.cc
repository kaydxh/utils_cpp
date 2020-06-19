#include "file_util.h"
#include <sys/file.h>

#include "core/scope_guard.h"

namespace kingfisher {
namespace fileutil {

int Open(const char *filename, int flags /* = O_RDWR | O_LARGEFILE | O_CREAT*/,
         mode_t mode /* = 0666 */) {
  return detail::WrapFuncT(open, filename, flags, mode);
}

ssize_t ReadFull(int fd, void *buf, size_t length) {
  return detail::WrapFileOpFuncT(read, fd, buf, length);
}

ssize_t WriteFull(int fd, const void *buf, size_t length) {
  return detail::WrapFileOpFuncT(write, fd, const_cast<void *>(buf), length);
}

bool CopyFile(const char *from_path, const char *to_path) {
  ssize_t in_fd = -1;
  ssize_t out_fd = -1;

  SCOPE_EXIT {
    if (in_fd != -1) {
      close(in_fd);
    }
    if (out_fd != -1) {
      close(out_fd);
    }
  };

  in_fd = Open(from_path, O_RDONLY);
  if (-1 == in_fd) {
    return false;
  }

  out_fd = Open(to_path);
  if (-1 == out_fd) {
    return false;
  }

  const size_t BUFFER_SIZE = 1024;
  char buf[BUFFER_SIZE] = {'\0'};

  int sz = 0;
  while ((sz = ReadFull(in_fd, buf, BUFFER_SIZE)) > 0) {
    if (sz != WriteFull(out_fd, buf, sz)) {
      return false;
    }
    std::cout << "----WriteFull: " << sz << std::endl;
  }

  return true;
}

bool DeleteFile(const char *filename) {
  if (nullptr == filename) {
    return false;
  }

  return 0 == ::unlink(filename);
}

int Flock(int fd, int operation) {
  return detail::WrapFuncT(flock, fd, operation);
}

}  // namespace fileutil
}  // namespace kingfisher
