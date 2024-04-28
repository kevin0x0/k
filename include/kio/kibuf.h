#ifndef KEVCC_UTILS_INCLUDE_KIO_KIBUF_H
#define KEVCC_UTILS_INCLUDE_KIO_KIBUF_H

#include "include/kio/ki.h"

Ki* kibuf_create(const void* buf, size_t bufsize);
Ki* kistr_create(const char* str);

#endif
