
#include "strings/strings.h"

#include <cctype>
#include <sstream>

namespace kingfisher {
namespace strings {

bool IsNumber(const std::string &s) {
  if (s.empty()) {
    return false;
  }

  for (size_t i = 0; i < s.length(); ++i) {
    if (!std::isdigit(s[i])) {
      if (i == 0 && s.length() > 1) {
        if (s[i] == '-' || s[i] == '+') {
          continue;
        }
      }
      return false;
    }
  }

  return true;
}

int ParseInt64(int64_t &result, const std::string &s, int base) {
  if (!IsNumber(s)) {
    return -1;
  }

  result = std::stoll(s);
  return 0;
}

int ParseUint64(uint64_t &result, const std::string &s, int base) {
  if (!IsNumber(s)) {
    return -1;
  }

  result = std::stoull(s);
  return 0;
}

std::vector<std::string> Split(const std::string &s, const char delim) {
  std::string word;
  std::stringstream ss(s);
  std::vector<std::string> words;

  while (std::getline(ss, word, delim)) {
    words.push_back(word);
  }
  return words;
}

}  // namespace strings
}  // namespace kingfisher
