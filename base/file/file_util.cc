#include "file_util.h"

namespace kingfisher {
namespace fileutil {

int Open(const char *filename, int flags /* = O_RDWR | O_LARGEFILE | O_CREAT*/,
         mode_t mode /* = 0666 */) {
  return detail::WrapFuncT(open, filename, flags, mode);
}

ssize_t ReadFull(int fd, void *buf, size_t length) {
  return detail::WrapFileOpFuncT(read, fd, buf, length);
}

}  // namespace fileutil
}  // namespace kingfisher
