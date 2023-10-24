#include "klang/include/value/klarray.h"
#include "klang/include/value/value.h"
#include "klang/include/value/typedecl.h"

#include <stdlib.h>

#define KLARRAY_DEFAULT_SIZE      (16)

bool klarray_init(KlArray* array) {
  if (!array) return false;
  if (!(array->begin = (KlValue**)malloc(sizeof (KlValue*) * KLARRAY_DEFAULT_SIZE))) {
    array->end = NULL;
    array->current = NULL;
    return false;
  }

  array->end = array->begin + KLARRAY_DEFAULT_SIZE;
  array->current = array->begin;
  return true;
}

bool klarray_init_copy(KlArray* array, KlArray* src) {
  if (!klarray_init(array))
    return false;
  KlArrayIter begin = klarray_iter_begin(src);
  KlArrayIter end = klarray_iter_end(src);
  for (KlArrayIter itr = begin; itr != end; itr = klarray_iter_next(itr)) {
    if (!klarray_push_back(array, *itr)) {
      klarray_destroy(array);
      return false;
    }
  }
  return true;
}

void klarray_destroy(KlArray* array) {
  if (!array) return;
  free(array->begin);
  array->begin = NULL;
  array->end = NULL;
  array->current = NULL;
}

KlArray* klarray_create(void) {
  KlArray* array = (KlArray*)malloc(sizeof (KlArray));
  if (!array || !klarray_init(array)) {
    klarray_delete(array);
    return NULL;
  }
  return array;
}

KlArray* klarray_create_copy(KlArray* src) {
  KlArray* array = (KlArray*)malloc(sizeof (KlArray));
  if (!array || !klarray_init_copy(array, src)) {
    klarray_delete(array);
    return NULL;
  }
  return array;
}

void klarray_delete(KlArray* array) {
  klarray_destroy(array);
  free(array);
}

bool klarray_expand(KlArray* array) {
  size_t new_size = klarray_size(array) * 2;
  KlValue** new_array = (KlValue**)realloc(array->begin, sizeof (KlValue*) * new_size);
  if (!new_array) return false;
  array->current = new_array + (array->current - array->begin);
  array->begin = new_array;
  array->end = new_array + new_size;
  return true;
}

