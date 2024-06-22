#include "include/kio/kifile.h"
#include "include/kio/kio_common.h"
#include "include/kutils/utils.h"
#include "include/os_spec/kfs.h"

#include <stdlib.h>

#define KIFILE_BUFSIZE  (8192)

typedef struct tagKiFile {
  Ki ki;
  FILE* file;
  size_t default_bufsize;
} KiFile;

typedef struct tagKiFileKeepContent {
  Ki ki;
  FILE* file;
  unsigned char* buf;
  unsigned char* curr;
  unsigned char* end;
  bool eof;
} KiFileKeepContent;

static KioFileOffset kifile_size(KiFile* kifile);
static void kifile_reader(KiFile* kifile);
static void kifile_close(KiFile* kifile);
static void kifile_detach(KiFile* kifile);
static void kifile_reader_keepcontent(KiFileKeepContent* kifile);
static void kifile_detach_keepcontent(KiFileKeepContent* kifile);

static const KiVirtualFunc kifile_create_vfunc = { .size = (KiSize)kifile_size, .delete = (KiDelete)kifile_close, .reader = (KiReader)kifile_reader };
static const KiVirtualFunc kifile_attach_vfunc = { .size = (KiSize)kifile_size, .delete = (KiDelete)kifile_detach, .reader = (KiReader)kifile_reader };
static const KiVirtualFunc kifile_attach_keepcontent_vfunc = { .size = (KiSize)kifile_size, .delete = (KiDelete)kifile_detach_keepcontent, .reader = (KiReader)kifile_reader_keepcontent };

Ki* kifile_create(const char* filepath, const char* mode) {
  FILE* file = fopen(filepath, mode);
  if (!file) return NULL;
  KiFile* kifile = (KiFile*)malloc(sizeof (KiFile));
  if (!kifile) {
    fclose(file);
    return NULL;
  }
  kifile->file = file;
  kifile->default_bufsize = KIFILE_BUFSIZE;
  ki_init((Ki*)kifile, &kifile_create_vfunc);
  return (Ki*)kifile;
}

Ki* kifile_attach(FILE* file) {
  KiFile* kifile = (KiFile*)malloc(sizeof (KiFile));
  if (!kifile) return NULL;
  kifile->file = file;
  kifile->default_bufsize = KIFILE_BUFSIZE;
  ki_init((Ki*)kifile, &kifile_attach_vfunc);
  return (Ki*)kifile;
}

Ki* kifile_attach_keepcontent(FILE* file) {
  KiFileKeepContent* kifile = (KiFileKeepContent*)malloc(sizeof (KiFileKeepContent));
  if (!kifile) return NULL;
  kifile->file = file;
  kifile->buf = NULL;
  kifile->curr = NULL;
  kifile->end = NULL;
  kifile->eof = false;
  ki_init((Ki*)kifile, &kifile_attach_keepcontent_vfunc);
  return (Ki*)kifile;
}

static void kifile_reader(KiFile* kifile) {
  KioFileOffset readpos = ki_tell((Ki*)kifile);
  if (fseek(kifile->file, readpos, SEEK_SET)) {
    ki_setbuf((Ki*)kifile, ki_getbuf((Ki*)kifile), 0, readpos);
    return;
  }
  void* buf = ki_getbuf((Ki*)kifile);
  if (!buf && !(buf = malloc(kifile->default_bufsize)))
      return;
  size_t readsize = fread(buf, 1, kifile->default_bufsize, kifile->file);
  ki_setbuf((Ki*)kifile, buf, readsize, readpos);
}

static void kifile_reader_keepcontent(KiFileKeepContent* kifile) {
  KioFileOffset readpos = ki_tell((Ki*)kifile);
  if (readpos < (KioFileOffset)(kifile->curr - kifile->buf) || kifile->eof) {
    ki_setbuf((Ki*)kifile, kifile->buf + readpos, kifile->curr - (kifile->buf + readpos), readpos);
    return;
  }
  if (readpos >= (KioFileOffset)(kifile->end - kifile->buf)) {
    size_t oldsize = kifile->curr - kifile->buf;
    size_t newcap = readpos + KIFILE_BUFSIZE;
    unsigned char* newbuf = realloc(kifile->buf, newcap);
    if (k_unlikely(!newbuf)) {
      ki_setbuf((Ki*)kifile, kifile->curr, 0, readpos);
      return;
    }
    kifile->end = newbuf + newcap;
    kifile->curr = newbuf + oldsize;
    kifile->buf = newbuf;
  }

  size_t readsize = fread(kifile->curr, 1, kifile->end - kifile->curr, kifile->file);
  kifile->eof = readsize != (size_t)(kifile->end - kifile->curr);
  kifile->curr += readsize;
  if (readpos >= (KioFileOffset)(kifile->curr - kifile->buf)) {
    ki_setbuf((Ki*)kifile, kifile->curr, 0, readpos);
    return;
  }
  ki_setbuf((Ki*)kifile, kifile->buf + readpos, kifile->curr - (kifile->buf + readpos), readpos);
  return;
}

static void kifile_close(KiFile* kifile) {
  fclose(kifile->file);
  free(ki_getbuf((Ki*)kifile));
  free(kifile);
}

static void kifile_detach(KiFile* kifile) {
  free(ki_getbuf((Ki*)kifile));
  free(kifile);
}

static void kifile_detach_keepcontent(KiFileKeepContent* kifile) {
  free(kifile->buf);
  free(kifile);
}

static KioFileOffset kifile_size(KiFile* kifile) {
  return kfs_file_size(kifile->file);
}
