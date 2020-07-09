set (
  base_SRC
  configparser/config_parser.cpp
  file/file.cc
  file/file_util.cc
  hash/md5.cpp
  serializer/djson.cpp
  thread/thread.cc
  thread/thread_pool.cc
  time/date.cc
  time/timestamp.cc
  time/time_zone.cc
  uuid/guid.cc
  )
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/configparser)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/hash)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/serializer)

add_library(kingfisher_base ${base_SRC})
install(TARGETS kingfisher_base DESTINATION lib)
