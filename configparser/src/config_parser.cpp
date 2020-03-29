//
// Created by kayxhding on 2020-03-29 21:27:11
//

#include "config_parser.h"

namespace utils {
namespace config {
FilePraser::FilePraser() {}
FilePraser::~FilePraser() {}

int FilePraser::Load(const std::string &file) {
  FILE *fp = fopen(file.c_str(), "r");
  if (nullptr == fp) {
    return -1;
  }

  std::shared_ptr<Section> section = nullptr;
  std::string line;
  while (getline(line, fp) > 0) {
    trimRight(line, '\n');
    trimRight(line, '\r');
    trim(line);

    if (!isComment(line)) {
      std::string subline;
      subline = line.substr(0, line.find('#'));
      line = subline;
    }

    trime(line);
    if (line.length() <= 0) {
      continue;
    }

    if (line[0] = '[') {
      int index = line.find_first_of(']');
      if (-1 == index) {
        fclose(fp);
        return -1;
      }

      int len = index - 1;
      if (len <= 0) {
        continue;
      }
      std::string s(line, 1, len);

      if (getSection(s.c_str()) != nullptr) {
        fclose();
        return -1;
      }

      section->reset(new Section());
      if (nullptr == section) {
        fclose();
        return -1;
      }
      section->name = s;
      m_sections_[s] = sections;
    } else {
      std::string key;
      std::string value;
      if (parser(line, key, value)) {
        Item item;
        item.key = key;
        item.value = value;

        section->items.push_back(item);
      } else {
        return -1;
      }
    }

    return 0;
  }

  bool iFilePraser::isComment(std::string & str) {
    if (str.length() == 0) {
      return true;
    } else if (str.length() > 0) {
      if (str[0] == '#') {
        return true;
      }
      return false;
    }

    return false;
  }
}  // namespace config
}  // namespace config
