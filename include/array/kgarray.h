#ifndef _K_INCLUDE_ARRAY_KGARRAY_H_
#define _K_INCLUDE_ARRAY_KGARRAY_H_

#include "include/kutils/utils.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#define kgarray_pass_ref(KType)     KType*
#define kgarray_pass_val(KType)     KType
#define kgarray_pass_ref_get(val)   *val
#define kgarray_pass_val_get(val)   val

#define kgarray_static              static
#define kgarray_nonstatic

#define kgarray_decl(T, arrname, prefix, pass, kstatic)                         \
typedef struct tag##arrname {                                                   \
  T* begin;                                                                     \
  T* end;                                                                       \
  T* current;                                                                   \
} arrname;                                                                      \
                                                                                \
kgarray_##kstatic bool prefix##_init(arrname* array, size_t size);              \
kgarray_##kstatic void prefix##_destroy(arrname* array);                        \
kgarray_##kstatic arrname* prefix##_create(size_t size);                        \
kgarray_##kstatic void prefix##_delete(arrname* array);                         \
                                                                                \
static inline bool prefix##_push_back(arrname* array, kgarray_##pass(T) val);   \
static inline void prefix##_pop_back(arrname* array, size_t npop);              \
static inline T* prefix##_back(const arrname* array);                           \
static inline T* prefix##_front(const arrname* array);                          \
kgarray_##kstatic bool prefix##_expand(arrname* array);                         \
kgarray_##kstatic bool prefix##_recap(arrname* array, size_t new_size);         \
static inline T* prefix##_access(const arrname* array, size_t index);           \
static inline void prefix##_setsize(arrname* array, size_t size);               \
static inline size_t prefix##_size(const arrname* array);                       \
static inline size_t prefix##_capacity(const arrname* array);                   \
/* shrink the array to exactly fit its size */                                  \
static inline void prefix##_shrink(arrname* array);                             \
static inline T* prefix##_steal(arrname* array);                                \
                                                                                \
static inline void prefix##_setsize(arrname* array, size_t size) {              \
  array->current = array->begin + size;                                         \
}                                                                               \
static inline size_t prefix##_size(const arrname* array) {                      \
  return array->current - array->begin;                                         \
}                                                                               \
                                                                                \
static inline size_t prefix##_capacity(const arrname* array) {                  \
  return array->end - array->begin;                                             \
}                                                                               \
                                                                                \
static inline void prefix##_shrink(arrname* array) {                            \
  size_t size = prefix##_size(array);                                           \
  size_t shrinksize = size == 0 ? 1 : size;                                     \
  T* newarr = (T*)realloc(array->begin, shrinksize * sizeof (T));               \
  if (k_likely(newarr)) {                                                       \
    array->begin = newarr;                                                      \
    array->current = array->begin + size;                                       \
    array->end = array->begin + shrinksize;                                     \
  }                                                                             \
}                                                                               \
                                                                                \
static inline bool prefix##_push_back(arrname* array, kgarray_##pass(T) val) {  \
  if (k_unlikely(array->current == array->end &&                                \
      !prefix##_expand(array))) {                                               \
    return false;                                                               \
  }                                                                             \
  *array->current++ = kgarray_##pass##_get(val);                                \
  return true;                                                                  \
}                                                                               \
                                                                                \
static inline void prefix##_pop_back(arrname* array, size_t npop) {             \
  array->current -= npop;                                                       \
}                                                                               \
                                                                                \
static inline T* prefix##_back(const arrname* array) {                          \
  return array->current - 1;                                                    \
}                                                                               \
                                                                                \
static inline T* prefix##_front(const arrname* array) {                         \
    return array->begin;                                                        \
}                                                                               \
                                                                                \
static inline T* prefix##_access(const arrname* array, size_t index) {          \
  return &array->begin[index];                                                  \
}                                                                               \
                                                                                \
static inline T* prefix##_steal(arrname* array) {                               \
  T* ret = array->begin;                                                        \
  array->begin = NULL;                                                          \
  array->current = NULL;                                                        \
  array->end = NULL;                                                            \
  return ret;                                                                   \
}



#define kgarray_impl(T, arrname, prefix, pass, kstatic)                         \
kgarray_##kstatic bool prefix##_init(arrname* array, size_t size) {             \
  if (k_unlikely(!array)) return false;                                         \
  if (k_unlikely(!(array->begin = (T*)malloc(sizeof (T) * size)))) {            \
    array->end = NULL;                                                          \
    array->current = NULL;                                                      \
    return false;                                                               \
  }                                                                             \
                                                                                \
  array->end = array->begin + size;                                             \
  array->current = array->begin;                                                \
  return true;                                                                  \
}                                                                               \
                                                                                \
kgarray_##kstatic void prefix##_destroy(arrname* array) {                       \
  if (k_unlikely(!array)) return;                                               \
  free(array->begin);                                                           \
  array->begin = NULL;                                                          \
  array->end = NULL;                                                            \
  array->current = NULL;                                                        \
}                                                                               \
                                                                                \
kgarray_##kstatic arrname* prefix##_create(size_t size) {                       \
  arrname* array = (arrname*)malloc(sizeof (arrname));                          \
  if (k_unlikely(!array || !prefix##_init(array, size))) {                      \
    prefix##_delete(array);                                                     \
    return NULL;                                                                \
  }                                                                             \
  return array;                                                                 \
}                                                                               \
                                                                                \
kgarray_##kstatic void prefix##_delete(arrname* array) {                        \
  prefix##_destroy(array);                                                      \
  free(array);                                                                  \
}                                                                               \
                                                                                \
kgarray_##kstatic bool prefix##_recap(arrname* array, size_t new_size) {        \
  size_t old_size = prefix##_size(array);                                       \
  T* new_array = (T*)realloc(array->begin, sizeof (T) * new_size);              \
  if (k_unlikely(!new_array)) return false;                                     \
  array->current = new_array + old_size;                                        \
  array->begin = new_array;                                                     \
  array->end = new_array + new_size;                                            \
  return true;                                                                  \
}                                                                               \
                                                                                \
kgarray_##kstatic bool prefix##_expand(arrname* array) {                        \
  size_t old_size = prefix##_size(array);                                       \
  return prefix##_recap(array, old_size * 2);                                   \
}                                                                               

#endif
