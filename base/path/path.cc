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

std::string Dir(const std::string &path) { return ""; }

int Split(const std::string &path, std::string &dir_return_,
          std::string &file_return_) {
  auto pos = path.find_last_of("/");
  if (pos != std::string::npos) {
    dir_return_ = path.substr(0, pos + 1);
    file_return_ = path.substr(pos + 1);
    return 0;
  }

  return -1;
}
}  // namespace path
}  // namespace kingfisher
