//
// Created by kayxhding on 2020-05-12 15:54:28
//

#ifndef SERIALIZER_BASE_RANDOM_RANDOM_H_
#define SERIALIZER_BASE_RANDOM_RANDOM_H_

#include <chrono>
#include <memory>
#include <random>
#include <type_traits>

namespace kingfisher {
namespace random {

class DefaultGenerator {
 public:
  DefaultGenerator() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    m_sp_rand_num.reset(new std::mt19937(seed));
  }
  ~DefaultGenerator() {}

  using result_type = uint32_t;

  static inline constexpr result_type min() {
    return std::numeric_limits<result_type>::min();
  }

  static inline constexpr result_type max() {
    return std::numeric_limits<result_type>::max();
  }
  uint32_t operator()() { return m_sp_rand_num->operator()(); }

 private:
  std::shared_ptr<std::mt19937> m_sp_rand_num;
};

class Random {
 private:
  template <typename RandomNumberGenerator>
  using ValidRNG =
      typename std::enable_if<std::is_unsigned<typename std::result_of<
                                  RandomNumberGenerator&()>::type>::value,
                              RandomNumberGenerator>::type;

 public:
  // using DefaultGenerator = std::mt19937;

  static inline uint32_t RandomNumberSeed() {
    // unsigned seed =
    // std::chrono::system_clock::now().time_since_epoch().count();
    // std::mt19937 rand_num(seed);
    return RandUInt32();
  }

  template <typename RandomNumberGenerator = DefaultGenerator>
  static void Seed(ValidRNG<RandomNumberGenerator>& rng);

  // rand uint32_t
  template <typename RandomNumberGenerator = DefaultGenerator>
  static inline uint32_t RandUInt32(
      ValidRNG<RandomNumberGenerator> rrng = RandomNumberGenerator()) {
    return rrng.operator()();
  }

  // rand [0, max-1)
  template <typename RandomNumberGenerator = DefaultGenerator>
  static inline uint32_t RandUInt32(
      uint32_t max,
      ValidRNG<RandomNumberGenerator> rrng = RandomNumberGenerator()) {
    if (0 == max) {
      return 0;
    }

    return std::uniform_int_distribution<uint32_t>(0, max - 1)(rrng);
  }

  // rand [min, max-1)
  template <typename RandomNumberGenerator = DefaultGenerator>
  static inline uint32_t RandUInt32(
      uint32_t min, uint32_t max,
      ValidRNG<RandomNumberGenerator> rrng = RandomNumberGenerator()) {
    if (min == max) {
      return 0;
    }

    return std::uniform_int_distribution<uint32_t>(min, max - 1)(rrng);
  }

  // rand uint64_t
  template <typename RandomNumberGenerator = DefaultGenerator>
  static inline uint64_t RandUInt64(
      ValidRNG<RandomNumberGenerator> rrng = RandomNumberGenerator()) {
    return ((uint64_t)rrng() << 32) | rrng();
  }

  // rand [0, max-1)
  template <typename RandomNumberGenerator = DefaultGenerator>
  static inline uint64_t RandUInt64(
      uint64_t max,
      ValidRNG<RandomNumberGenerator> rrng = RandomNumberGenerator()) {
    if (0 == max) {
      return 0;
    }

    return std::uniform_int_distribution<uint64_t>(0, max - 1)(rrng);
  }

  // rand [min, max-1)
  template <typename RandomNumberGenerator = DefaultGenerator>
  static inline uint64_t RandUInt64(
      uint64_t min, uint64_t max,
      ValidRNG<RandomNumberGenerator> rrng = RandomNumberGenerator()) {
    if (min == max) {
      return 0;
    }

    return std::uniform_int_distribution<uint64_t>(min, max - 1)(rrng);
  }
};

}  // namespace random
}  // namespace kingfisher

#endif
