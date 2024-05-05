#ifndef _K_INCLUDE_KIO_KIBUF_H_
#define _K_INCLUDE_KIO_KIBUF_H_

#include "include/kio/ki.h"

Ki* kibuf_create(const void* buf, size_t bufsize);
Ki* kistr_create(const char* str);

#endif
