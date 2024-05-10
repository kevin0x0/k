#ifndef _K_INCLUDE_KUTILS_UTILS_H_
#define _K_INCLUDE_KUTILS_UTILS_H_

#include <assert.h>

#define k_likely(expr)    (__builtin_expect(!!(expr), 1))
#define k_unlikely(expr)  (__builtin_expect(!!(expr), 0))

#define k_assert(expr)    assert(expr)


#endif
