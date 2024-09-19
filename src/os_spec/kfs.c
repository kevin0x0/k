#include "include/os_spec/kfs.h"
#include "include/kutils/utils.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#else
#include <unistd.h>
#endif

char* kfs_get_bin_dir(void) {
  char* buf = NULL;
  size_t buf_size = 64;
#ifdef _WIN32
  size_t len = 0;
#else
  int len = 0;
#endif
  do {
    free(buf);
    buf_size = buf_size + buf_size / 2;
    buf = (char*)malloc(sizeof (char) * buf_size);
    if (!buf) return NULL;
#ifdef _WIN32
  } while ((len = (size_t)GetModuleFileNameA(NULL, buf, buf_size)) == buf_size);
#else
  } while ((len = readlink("/proc/self/exe",buf, buf_size)) == -1);
#endif
  len--;
#ifdef _WIN32
  for (char* ptr = buf; *ptr != '\0'; ++ptr)
    if (*ptr == '\\') *ptr = '/';
#endif
  while (buf[--len] != '/') continue;
  buf[len + 1] = '\0';
  return buf;
}

char* kfs_get_relpath(const char* from, const char* to) {
  size_t i = 0;
  while (from[i] == to[i] && from[i] != '\0')
    i++;
  while (i != 0 && from[--i] != '/' && from[i] != '\\')
    continue;
  if (i != 0) ++i;
  size_t dir_depth = 0;
  for (size_t j = i; from[j] != '\0'; ++j) {
    if (from[j] == '/' || from[j] == '\\')
      dir_depth++;
  }
  char* relpath = (char*)malloc(sizeof (char) * (dir_depth * 3 + strlen(to) + 1));
  if (!relpath) return NULL;
  for (size_t j = 0; j < dir_depth; ++j) {
    relpath[j * 3] = '.';
    relpath[j * 3 + 1] = '.';
    relpath[j * 3 + 2] = '/';
  }
  relpath[dir_depth * 3] = '\0';
  strcat(relpath, to + i);
  return relpath;
}

char* kfs_trunc_leaf(const char* path) {
  size_t len = strlen(path);
  char* cp_path = (char*)malloc((len + 3) * sizeof (char));
  if (!cp_path) return NULL;
  strcpy(cp_path, path);
  size_t i = len;
  while (--i && cp_path[i] != '\\' && cp_path[i] != '/')
    continue;
  if (i == 0) {
    cp_path[0] = '.';
    cp_path[1] = '/';
    cp_path[2] = '\0';
  } else {
    cp_path[i + 1] = '\0';
  }
  return cp_path;
}

char* kfs_getcwd(void) {
  char* buf = NULL;
  size_t size = 128;
  while (true) {
    char* newbuf = (char*)realloc(buf, size + 1);
    if (!newbuf) {
      free(buf);
      return NULL;
    }
    buf = newbuf;
#ifdef _WIN32
    if (_getcwd(buf, size)) {
#else
    if (getcwd(buf, size)) {
#endif
      char* p = buf;
      while (*p) {
        if (*p == '\\') *p = '/';
        ++p;
      }
      if (*(p - 1) != '/') {
        *p++ = '/';
        *p = '\0';
      }
      return buf;
    }
    if (errno != ERANGE) {
      free(buf);
      return NULL;
    }
    size = size + size / 2;
  }
}

char* kfs_abspath(const char* path) {
  if (!kfs_is_relative(path)) {
    char* res = (char*)malloc(strlen(path) + 1);
    if (k_unlikely(!res)) return NULL;
    strcpy(res, path);
    return res;
  }
  char* cwd = kfs_getcwd();
  if (k_unlikely(!cwd)) return NULL;
  size_t pathlen = strlen(path);
  size_t cwdlen = strlen(cwd);
  size_t abspathlen = cwdlen + pathlen;
  char* abspath = (char*)realloc(cwd, abspathlen + 1);
  if (k_unlikely(!abspath)) {
    free(cwd);
    return NULL;
  }
  memcpy(abspath + cwdlen, path, pathlen + 1);
  return abspath;
}

bool kfs_is_relative(const char* path) {
#ifdef _WIN32
  return !isupper(path[0]) || path[1] != ':';
#else
  return path[0] != '/';
#endif
}

#ifdef _WIN32
#include <windows.h>
#include <fileapi.h>

size_t kfs_file_size(FILE* file) {
  HANDLE hfile = (HANDLE)_get_osfhandle(_fileno(file));
  LARGE_INTEGER li;
  GetFileSizeEx(hfile, &li);
  size_t size = li.QuadPart;
  return size;
}

#else
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>

uintmax_t kfs_file_size(FILE* file) {
  int fd = fileno(file);
  struct stat file_state;
  fstat(fd, &file_state);
  return file_state.st_size;
}

#endif
