#include "os/getwd.h"

#include <unistd.h>

namespace kingfisher {
namespace os {
std::string Getwd() {
#if 0
  char *buff = nullptr;
  buff = getcwd(nullptr, 0);

  std::string path(buff);

  if (buff != nullptr) {
    free(buff);
  }
  return path;
#endif
  char *buff = nullptr;
  buff = get_current_dir_name();
  std::string path(buff);
  return path;
}

}  // namespace os
}  // namespace kingfisher
