#ifndef _K_INCLUDE_KIO_KIFILE_H_
#define _K_INCLUDE_KIO_KIFILE_H_

#include "include/kio/ki.h"
#include <stdio.h>


Ki* kifile_create(const char* filepath, const char* mode);
Ki* kifile_attach(FILE* file);
Ki* kifile_attach_stdin(FILE* file);
Ki* kifile_attach_keepcontent(FILE* file);

#endif
