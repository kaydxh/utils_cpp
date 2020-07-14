//
// Created by kayxhding on 2020-03-30 23:28:52
//

#include <iostream>
#include "config_parser.h"

int main() {
  std::string file = "./conf/config.toml";
  std::string section_name = "app";
  utils::config::FileParser file_parser;
  int ret = file_parser.Load(file);
  if (ret != 0) {
    std::cout << "load failed, ret: " << ret << std::endl;
    return -1;
  }
  std::string value;
  std::string port;
  file_parser.getValue("app1", "addr", value);
  file_parser.getValue("app2", "port", port);
  std::cout << "value:" << value << std::endl;
  std::cout << "port:" << port << std::endl;

  return 0;
}
