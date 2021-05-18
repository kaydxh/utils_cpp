#ifndef KINNGFISHER_BASE_CHECK_CHECK_H_
#define KINNGFISHER_BASE_CHECK_CHECK_H_

#define CHECK(condition, result) \
  do {                           \
    if (!(condition)) {          \
      return result;             \
    }                            \
  } while (0);

#endif
