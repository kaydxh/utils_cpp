#ifndef KINGFISHER_BASE_PATH_PATH_H_
#define KINGFISHER_BASE_PATH_PATH_H_

#include <string>

namespace kingfisher {
namespace path {

std::string Abs(const std::string &path);

bool IsAbs(const std::string &path);

}  // namespace path
}  // namespace kingfisher

#endif
