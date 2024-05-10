#ifndef _K_INCLUDE_STRING_KSTRING_H_
#define _K_INCLUDE_STRING_KSTRING_H_

#include <stdbool.h>
#include <stddef.h>


size_t kstr_len(const char* str);
char* kstr_copy(const char* str);
char* kstr_concat(const char* str1, const char* str2);
char* kstr_copy_len(const char* str, size_t len);
char* kstr_concat_len(const char* str1, const char* str2, size_t len1, size_t len2);
static inline bool kstr_is_prefix(const char* prefix, const char* str);
static inline size_t kstr_prefix(const char* prefix, const char* str);

static inline size_t kstr_prefix(const char* prefix, const char* str) {
  size_t i = 0;
  while (prefix[i] == str[i] && prefix[i] != '\0') ++i;
  return i;
}

static inline bool kstr_is_prefix(const char* prefix, const char* str) {
  return prefix[kstr_prefix(prefix, str)] == '\0';
}

#endif
