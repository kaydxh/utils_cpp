//
// Created by kayxhding on 2020-08-27 17:26:29
//

#ifndef KINNGFISHER_BASE_DUMP_STACK_TRACK_H_
#define KINNGFISHER_BASE_DUMP_STACK_TRACK_H_
#include <string>

namespace kingfisher {
namespace dump {

class StackTrace {
 public:
  StackTrace();
  ~StackTrace();

  const char* GetStackTrace();

 private:
  void demangleSymbol(std::string& symbol);
  // void processStackTrack();

 private:
  std::string stack_;
};

}  // namespace dump
}  // namespace kingfisher

#endif
