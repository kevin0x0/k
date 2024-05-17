#ifndef _K_INCLUDE_LIB_LIB_H_
#define _K_INCLUDE_LIB_LIB_H_
#include <stddef.h>

#ifdef _WIN32
#include <windows.h>
#endif

/* Due to platform-specificity, this file defines some wrapper functions for library loader */
typedef struct tagKLib {
#ifdef _WIN32
  HMODULE handle;
#else
  void* handle;
#endif
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
