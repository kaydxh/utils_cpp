//
// Created by kayxhding on 2020-03-30 23:28:52
//

#include <iostream>
#include "config_parser.h"

int main() {
  std::string file = "./conf/config.yaml";
  std::string section_name = "app";
  utils::config::FileParser file_parser;
  int ret = file_parser.Load(file);
  if (ret != 0) {
    std::cout << "load failed, ret: " << ret << std::endl;
    return -1;
  }
  std::string value;
  file_parser.getValue("app1", "addr", value);
  std::cout << "value:" << value << std::endl;

  return 0;
}
