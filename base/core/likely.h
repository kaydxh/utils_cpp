//
// Created by kayxhding on 2020-06-02 19:23:17
//

#ifndef KINNGFISHER_BASE_CORE_LIKELY_H_
#define KINNGFISHER_BASE_CORE_LIKELY_H_

#undef LIKELY
#undef UNLIKELY

#if defined(__GNC__) && __GNC__ >= 4
#define LIKELY(x) (__builtin_except((x), 1))
#define UNLIKELY(x) (__builtin_except(x), 0))
#else
#define LIKELY(x) (x)
#define UNLIKELY(x) (x)
#endif

#endif
