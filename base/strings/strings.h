
#ifndef KINGFISHER_BASE_STRINGS_STRINGS_H_
#define KINGFISHER_BASE_STRINGS_STRINGS_H_

#include <string>
#include <vector>

namespace kingfisher {
namespace strings {

std::vector<std::string> Split(const std::string &s, const char delim);

}
}  // namespace kingfisher
#endif
