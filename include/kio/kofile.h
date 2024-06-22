#ifndef _K_INCLUDE_KIO_KOFILE_H_
#define _K_INCLUDE_KIO_KOFILE_H_

#include "include/kio/ko.h"
#include <stdio.h>


Ko* kofile_create(const char* filepath, const char* mode);
Ko* kofile_attach(FILE* file);



#endif
