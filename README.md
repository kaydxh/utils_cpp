# kingfinsher
kingfinsher is a C++ based library

## 1 configparser
> configparser can prase the config file which is toml format.
```
# config file
[app1]
addr=127.0.0.1
port=8001
[app2]
addr=127.0.0.2
port=8002
```

### 1.1 usage
```
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
```

## 2 hash
> It can easily get hash value for string
```
  string str = "abc";
  auto hash = kingfisher::hash::Md5SumString(str);
  std::cout << hash << std::endl;
```
