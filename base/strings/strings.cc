
#include "strings/strings.h"

#include <sstream>

namespace kingfisher {
namespace strings {

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
