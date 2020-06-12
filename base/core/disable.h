//
// Created by kayxhding on 2020-06-11 17:22:13
//

#ifndef KINNGFISHER_BASE_CORE_DISABLE_H_
#define KINNGFISHER_BASE_CORE_DISABLE_H_

#define DISABLE_COPY(ClassName) ClassName(const ClassName &) = delete

#define DISABLE_ASSIGN(ClassName) \
  ClassName &operator=(const ClassName &) = delete

#define DISABLE_COPY_AND_ASSIGN(ClassName) \
  DISABLE_COPY(ClassName);                 \
  DISABLE_ASSIGN(ClassName)

#define DISABLE_IMPLICIT_CONSTRUCTORS(ClassName) \
  ClassName() = delete;                          \
  DISABLE_COPY_AND_ASSIGN(ClassName)

template <typename T>
inline void ignore_result(const T &) {}

#endif
