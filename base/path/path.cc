#include "path/path.h"

namespace kingfisher {
namespace path {

std::string Abs(const std::string &path) {
  char abs_path[256] = {0};
  if (realpath(path.c_str(), abs_path)) {
    return std::string(abs_path);
  }

  return "";
}

bool IsAbs(const std::string &path) {
  return path.length() > 0 && path[0] == '/';
}

}  // namespace path
}  // namespace kingfisher
