#ifndef K_INCLUDE_KUTILS_UTILS_H
#define K_INCLUDE_KUTILS_UTILS_H

#define k_likely(expr)    (__builtin_expect(!!(expr), 1))
#define k_unlikely(expr)  (__builtin_expect(!!(expr), 0))


#endif
