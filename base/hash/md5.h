//
// Created by kayxhding on 2020-04-15 13:54:29
//

#ifndef HASH_MD5_H
#define HASH_MD5_H

#include <stdint.h>
#include <cstring>
#include <string>

namespace kingfisher {
namespace hash {

struct Context {
  uint32_t state[4];
  uint32_t count[2];
  unsigned char buffer[64];
};

struct MD5Digest {
  MD5Digest() { Init(); }
  inline void Init() { memset(digest, 0, sizeof(digest)); }
  unsigned char digest[16];
};

class MD5 {
 public:
  MD5();
  ~MD5();

 public:
  std::string Md5SumString(const std::string &str);
  void md5Sum(const std::string &str, MD5Digest &digest);

 public:
  static const int32_t blocksize = 64;

 private:
  void init(Context *ctx);
  void update(Context *ctx, const unsigned char input[], size_t length);
  void update(Context *ctx, const char input[], size_t length);
  void finalize(Context *ctx, MD5Digest *digest);
  void transform(uint32_t state[4], const unsigned char block[]);
  void encode(unsigned char output[], const uint32_t input[], size_t len);
  void decode(uint32_t output[], const unsigned char input[], size_t len);
  std::string hexdigest(const MD5Digest &digest) const;

  static inline uint32_t F(uint32_t x, uint32_t y, uint32_t z);

  static inline uint32_t G(uint32_t x, uint32_t y, uint32_t z);

  static inline uint32_t H(uint32_t x, uint32_t y, uint32_t z);
  static inline uint32_t I(uint32_t x, uint32_t y, uint32_t z);
  // rotate_left rotates x left n bits.
  static inline uint32_t rotate_left(uint32_t x, int n);

  // FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
  // Rotation is separate from addition to prevent recomputation.
  static inline void FF(uint32_t &a, uint32_t b, uint32_t c, uint32_t d,
                        uint32_t x, uint32_t s, uint32_t ac);

  static inline void GG(uint32_t &a, uint32_t b, uint32_t c, uint32_t d,
                        uint32_t x, uint32_t s, uint32_t ac);

  static inline void HH(uint32_t &a, uint32_t b, uint32_t c, uint32_t d,
                        uint32_t x, uint32_t s, uint32_t ac);

  static inline void II(uint32_t &a, uint32_t b, uint32_t c, uint32_t d,
                        uint32_t x, uint32_t s, uint32_t ac);
};

std::string Md5SumString(const std::string &str);
}  // namespace hash
}  // namespace kingfisher

#endif
