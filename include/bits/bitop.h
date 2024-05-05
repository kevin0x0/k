#ifndef _K_INCLUDE_BITS_BITOP_H_
#define _K_INCLUDE_BITS_BITOP_H_

#if defined (__GNUC__) || defined (__clang__)
#define kbit_ctz(x)         (__builtin_ctzll(x))
#define kbit_popcount(x)    (__builtin_popcountll(x))
#else
#error "TODO: give software implementation for bit operation"
#endif

#endif
