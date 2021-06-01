#ifndef KINGFISHER_BASE_PATH_PATH_H_
#define KINGFISHER_BASE_PATH_PATH_H_

#include <string>

namespace kingfisher {
namespace path {

std::string Abs(const std::string &path);

bool IsAbs(const std::string &path);

std::string Dir(const std::string &path);

int Split(const std::string &path, std::string &dir_return_,
          std::string &file_return_);

}  // namespace path
}  // namespace kingfisher

#endif
