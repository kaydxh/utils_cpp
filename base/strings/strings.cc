
#include "strings/strings.h"

#include <cctype>
#include <sstream>

namespace kingfisher {
namespace strings {

bool IsNumber(const std::string &s) {
  for (char const &c : s) {
    if (!std::isdigit(c)) {
      return false;
    }
  }

  return true;
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
