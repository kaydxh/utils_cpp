//
// Created by kayxhding on 2020-05-20 15:29:31
//

#include "guid.h"
#include <iomanip>
#include <sstream>
#include "random/random.h"

namespace kingfisher {
namespace utils {

std::string Guid::GenUuidString() {
  uint64_t most_sign = kingfisher::random::Random::RandUInt64();
  uint64_t least_sign = kingfisher::random::Random::RandUInt64();

  std::stringstream stream;
  stream << std::setfill('0') << std::hex << std::uppercase << std::setw(8)
         << static_cast<uint32_t>(most_sign >> 32) << "-" << std::setw(4)
         << static_cast<uint32_t>((most_sign >> 16) & 0x0000ffff) << "-"
         << std::setw(4) << static_cast<uint32_t>(most_sign & 0x0000ffff) << "-"
         << std::setw(4) << static_cast<uint32_t>(least_sign >> 48) << "-"
         << std::setw(12) << (least_sign & 0x0000ffffffffffffULL);

  return stream.str();
}

}  // namespace utils
}  // namespace kingfisher
