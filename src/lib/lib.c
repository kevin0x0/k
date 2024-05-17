#include "include/lib/lib.h"

#ifdef _WIN32
#include <windows.h>

KLib klib_dlopen(const char* libname) {
  return (KLib) { .handle = LoadLibraryA(libname) };
}

void* klib_dlsym(KLib handle, const char* symname) {
  return GetProcAddress(handle.handle, symname);
}

int klib_dlclose(KLib handle) {
  return FreeLibrary(handle.handle);
}


#else

#include <dlfcn.h>

KLib klib_dlopen(const char* libname) {
  return (KLib) { .handle = dlopen(libname, RTLD_LAZY) };
}

void* klib_dlsym(KLib handle, const char* symname) {
  return dlsym(handle.handle, symname);
}

int klib_dlclose(KLib handle) {
  return dlclose(handle.handle);
}

#endif
