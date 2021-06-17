cmake_minimum_required (VERSION 3.18.1)

project (kingfisher CXX)

if(NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE "Debug")
endif()

set(CXX_FLAGS
 -g
 -DCHECK_PTHREAD_RETURN_VALUE
 -D_FILE_OFFSET_BITS=64
 -Wall
 -Wextra
 -Werror
 # -Wconversion
  -Wno-unused-parameter
 # -Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
 -Wshadow
 -Wwrite-strings
 -march=native
 -std=c++11
 -rdynamic
 )

# generate compile_commands.json
set (CMAKE_EXPORT_COMPILE_COMMANDS "ON")
set (CMAKE_CXX_COMPILER "g++")
set (CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -D__STDC_FORMAT_MACROS -fprofile-arcs -ftest-coverage -fPIC")
set (CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -finline-limit=1000 -DNDEBUG -D__STDC_FORMAT_MACROS")
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set (CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR})
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
#set_target_properties(kingfisher_base PROPERTIES PUBLIC_HEADER "*.h")

string (TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

include_directories (.)
include_directories (base)
include_directories (third_party)

add_subdirectory (base)

LINK_DIRECTORIES(${CMAKE_INSTALL_PREFIX}/lib)
add_subdirectory (test/base)

#INSTALL(TARGETS kingfisher_base 
#        LIBRARY DESTINATION ${LIBRARY_OUTPUT_PATH}
#        PUBLIC_HEADER DESTINATION ${PROJECT_BINARY_DIR}/include
#)
