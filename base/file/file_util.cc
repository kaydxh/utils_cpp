#include "file_util.h"
#include <sys/file.h>

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
