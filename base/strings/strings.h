
#ifndef KINGFISHER_BASE_STRINGS_STRINGS_H_
#define KINGFISHER_BASE_STRINGS_STRINGS_H_

#include <string>
#include <vector>

namespace kingfisher {
namespace strings {

bool IsNumber(const std::string &s);

int ParseInt64(int64_t &result, const std::string &s, int base = 10);

int ParseUint64(uint64_t &result, const std::string &s, int base = 10);

std::vector<std::string> Split(const std::string &s, const char delim);

bool HasPrefix(const std::string &s, const std::string &prefix,
               bool case_sensitive = true);
bool HasSuffix(const std::string &s, const std::string &suffix,
               bool case_sensitive = true);

std::string ToLower(const std::string &s);
std::string TrimLeft(const std::string &s, const std::string &prefix);
std::string TrimRight(const std::string &s, const std::string &suffix);

}  // namespace strings
}  // namespace kingfisher
#endif
