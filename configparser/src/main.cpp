//
// Created by kayxhding on 2020-03-30 23:28:52
//

#include "config_parser.h"

int main() {
  std::string file = "./conf/config.conf";
  std::string section_name = "app";
  utils::config::FileParser file_parser;
  int ret = file_parser.Load(file);
  if (ret != 0) {
    std::cout << "load failed, ret: " << ret << std::endl;
    return -1;
  }

  return 0;
}
