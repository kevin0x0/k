#include "include/kio/kibuf.h"
#include "include/kio/kio_common.h"
#include <stdlib.h>
#include <string.h>

typedef struct tagKiBuf {
  Ki ki;
  const void* buf;
  size_t bufsize;
} KiBuf;

static void kibuf_delete(KiBuf* kibuf);
static KioFileOffset kibuf_size(KiBuf* kibuf);
static void kibuf_reader(KiBuf* kibuf);

static const KiVirtualFunc kibuf_vfunc = { .reader = (KiReader)kibuf_reader, .delete = (KiDelete)kibuf_delete, .size = (KiSize)kibuf_size };


Ki* kibuf_create(const void* buf, size_t bufsize) {
  KiBuf* kibuf = (KiBuf*)malloc(sizeof (KiBuf));
  if (!kibuf) return NULL;
  kibuf->buf = buf;
  kibuf->bufsize = bufsize;
  ki_init((Ki*)kibuf, &kibuf_vfunc);
  ki_setbuf((Ki*)kibuf, buf, bufsize, 0);
  return (Ki*)kibuf;
}

static void kibuf_delete(KiBuf* kibuf) {
  free(kibuf);
}

static KioFileOffset kibuf_size(KiBuf* kibuf) {
  return kibuf->bufsize;
}

static void kibuf_reader(KiBuf* kibuf) {
  size_t readpos = ki_tell((Ki*)kibuf);
  if (readpos >= kibuf->bufsize) {
    ki_setbuf((Ki*)kibuf, ki_getbuf((Ki*)kibuf), 0, readpos);
    return;
  }
  ki_setbuf((Ki*)kibuf, kibuf->buf, kibuf->bufsize, 0);
  ki_setbufcurr((Ki*)kibuf, readpos);
}

Ki* kistr_create(const char* str) {
  return kibuf_create(str, strlen(str));
}
