//
// Created by kayxhding on 2020-03-29 20:57:42
//
#ifndef UTILS_CONFIGPARSER_INCLUDE_CONFIG_PARSER_H
#define UTILS_CONFIGPARSER_INCLUDE_CONFIG_PARSER_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace utils {
namespace config {
struct Item {
  std::string key;
  std::string value;
};

struct Section {
  std::string name;
  std::vector<Item> items;
};

// template <typename T>
class FileParser {
 public:
  FileParser();
  ~FileParser();

 public:
  //  using std::unordered_map<std::string, section *>::iterator = iterator;
  int Load(const std::string &file);
  void GetSections(std::vector<std::string> &section_names);
  // T GetItem(const std::string &key, std::string &default_value = "");

 private:
  int getline(std::string &str, FILE *fp);
  void trim(std::string &str);
  void trimLeft(std::string &str, char c = ' ');
  void trimRight(std::string &str, char c = ' ');
  bool isComment(const std::string &str);
  std::shared_ptr<Section> getSection(const std::string &section_name);
  bool parser(const std::string &content, std::string &key, std::string &value);

 private:
  std::unordered_map<std::string, std::shared_ptr<Section>> m_sections_;
};
}  // namespace config
}  // namespace utils
#endif
