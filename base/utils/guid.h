//
// Created by kayxhding on 2020-05-06 17:12:54
//

#ifndef KINNGFISHER_BASE_UTILS_UTILS_H_
#define KINNGFISHER_BASE_UTILS_UTILS_H_

#include <memory>
#include <mutex>
#include <random>
#include <string>

namespace kingfisher {
namespace utils {

class Guid {
 public:
  Guid();
  Guid(const std::string &guid);
  ~Guid() {}

 public:
  static std::string GenUuidString();

 private:
  void init();

 private:
  static std::mutex m_guid_mutex;
  static std::shared_ptr<std::mt19937> m_sp_random_generator;
  static std::shared_ptr<std::uniform_int_distribution<uint64_t>>
      m_sp_distribution;
};

}  // namespace utils
}  // namespace kingfisher

#endif
