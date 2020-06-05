#include "file_util.h"

namespace kingfisher {
namespace fileutil {

ssize_t ReadFull(int fd, void *buf, size_t length) {
  return detail::WrapFileOpFuncT(read, fd, buf, length);
}

}  // namespace fileutil
}  // namespace kingfisher
