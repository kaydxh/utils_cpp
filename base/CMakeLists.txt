set (
  base_SRC
  configparser/config_parser.cpp
  hash/md5.cpp
  serializer/djson.cpp
  )
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/configparser)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/hash)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/serializer)

add_library(kingfish_base ${base_SRC})
