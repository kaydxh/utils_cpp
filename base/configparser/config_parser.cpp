//
// Created by kayxhding on 2020-03-29 21:27:11
//

#include "config_parser.h"
#include <string.h>

namespace utils {
namespace config {
const int kInitBufSize = 2048;
FileParser::FileParser() {}
FileParser::~FileParser() {}

int FileParser::Load(const std::string &file) {
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

    trim(line);
    if (line.length() <= 0) {
      continue;
    }

    if (line[0] == '[') {
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
        fclose(fp);
        return -1;
      }

      section.reset(new Section());
      if (nullptr == section) {
        fclose(fp);
        return -1;
      }
      section->name = s;
      m_sections_[s] = section;
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
  }

  return 0;
}

int FileParser::getValue(const std::string &section_name,
                         const std::string &key, std::string &value) {
  auto spSection = getSection(section_name);
  if (spSection == nullptr) {
    return -1;
  }

  for (auto it = spSection->items.begin(); it != spSection->items.end(); ++it) {
    if (it->key == key) {
      value = it->value;
      return 0;
    }
  }

  return -1;
}

int FileParser::getline(std::string &str, FILE *fp) {
  int plen = 0;
  int buf_size = kInitBufSize * sizeof(char);
  char *buf = reinterpret_cast<char *>(malloc(buf_size));
  char *pbuf = nullptr;
  char *p = buf;
  if (buf == nullptr) {
    exit(-1);
  }
  memset(buf, 0, buf_size);
  int total_size = buf_size;

  while (fgets(p, buf_size, fp) != nullptr) {
    plen = strlen(p);

    if (plen > 0 && p[plen - 1] != '\n' && !feof(fp)) {
      total_size = strlen(buf) + buf_size;
      pbuf = reinterpret_cast<char *>(realloc(buf, total_size));
      if (pbuf == nullptr) {
        free(pbuf);

        exit(-1);
      }

      buf = pbuf;
      p = buf + strlen(buf);
      continue;

    } else {
      break;
    }
  }
  str = buf;
  free(buf);
  buf = nullptr;
  return str.length();
}

bool FileParser::isComment(const std::string &str) {
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
void FileParser::trim(std::string &str) {
  trimRight(str);
  trimLeft(str);
}

void FileParser::trimLeft(std::string &str, char c) {
  int i = 0;
  int len = str.length();
  while (str[i] == c && str[i] != '\0') {
    i++;
  }
  if (i != 0) {
    str = str.substr(i, len - i);
  }
}

void FileParser::trimRight(std::string &str, char c) {
  int i = 0;
  int len = str.length();
  while (str[len - i - 1] == c && i >= 0) {
    i++;
  }
  if (i != 0) {
    str = str.substr(0, len - i);
  }
}

bool FileParser::parser(const std::string &content, std::string &key,
                        std::string &value) {
  int i = 0;
  int len = content.length();
  while (i < len && content[i] != '=') {
    i++;
  }
  if (i > 0 && i < len - 1) {
    key = content.substr(0, i);
    value = content.substr(i + 1);
    return true;
  }
  return false;
}
std::shared_ptr<Section> FileParser::getSection(
    const std::string &section_name) {
  auto it = m_sections_.find(section_name);
  if (it != m_sections_.end()) {
    return it->second;
  }

  return nullptr;
}
}  // namespace config
}  // namespace utils
