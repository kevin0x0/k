#ifndef KEVCC_UTILS_INCLUDE_LIB_H
#define KEVCC_UTILS_INCLUDE_LIB_H
#include <stddef.h>

/* Due to platform-specificity, this file defines some wrapper functions for library loader */
typedef struct tagKLib {
  void* handle;
}KLib;

KLib klib_dlopen(const char* libname);
int klib_dlclose(KLib handle);
void* klib_dlsym(KLib handle, const char* symname);

static inline int klib_success(KLib handle) {
  return handle.handle != NULL;
}

static inline int klib_failed(KLib handle) {
  return handle.handle == NULL;
}

#endif
