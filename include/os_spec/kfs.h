#ifndef KEVCC_UTILS_INCLUDE_FS_H
#define KEVCC_UTILS_INCLUDE_FS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/* Due to platform-specificity, this file defines some wrapper functions
 * related to directory operations. */

char* kfs_get_bin_dir(void);
char* kfs_get_relpath(const char* from, const char* to);
char* kfs_trunc_leaf(const char* path);
bool kfs_is_relative(const char* path);
size_t kfs_file_size(FILE* file);

#endif
