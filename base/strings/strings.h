
#ifndef KINGFISHER_BASE_STRINGS_STRINGS_H_
#define KINGFISHER_BASE_STRINGS_STRINGS_H_

#include <string>
#include <vector>

namespace kingfisher {
namespace strings {

bool IsNumber(const std::string &s);
std::vector<std::string> Split(const std::string &s, const char delim);

}  // namespace strings
}  // namespace kingfisher
#endif
