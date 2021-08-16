
#include "strings/strings.h"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>

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

bool HasPrefix(const std::string &s, const std::string &prefix,
               bool case_sensitive) {
  if (s.length() < prefix.length()) {
    return false;
  }

  std::string s_lower = s;
  std::string prefix_lower = prefix;
  if (!case_sensitive) {
    s_lower = ToLower(s);
    prefix_lower = ToLower(prefix);
  }

  auto pos = s_lower.find(prefix_lower);
  return 0 == pos;
}

bool HasSuffix(const std::string &s, const std::string &suffix,
               bool case_sensitive) {
  if (s.length() < suffix.length()) {
    return false;
  }

  std::string s_lower = s;
  std::string suffix_lower = suffix;
  if (!case_sensitive) {
    s_lower = ToLower(s);
    suffix_lower = ToLower(suffix);
  }

  auto pos = s_lower.rfind(suffix_lower);
  return pos == s.length() - suffix.length();
}

std::string ToLower(const std::string &s) {
  std::string lower;
  std::transform(s.begin(), s.end(), lower.begin(), ::tolower);
  return lower;
}

/**
 * Returns a substring with all characters the provided @toTrim returns true
 * for removed from the front of @sp.
 */
template <typename ToTrim>
std::string ltrim(std::string s, ToTrim toTrim) {
  while (!s.empty() && toTrim(s.front())) {
    s = s.substr(1);
  }

  return s;
}

std::string TrimLeft(const std::string &s, const std::string &prefix) {
  if (HasPrefix(s, prefix)) {
    return TrimLeft(s.substr(prefix.length()), prefix);
  }
  return s;
}

std::string TrimRight(const std::string &s, const std::string &suffix) {
  if (HasSuffix(s, suffix)) {
    return TrimRight(s.substr(0, s.length() - suffix.length()), suffix);
  }
  return s;
}

}  // namespace strings
}  // namespace kingfisher
