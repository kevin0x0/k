#include "include/kio/kofile.h"
#include "include/kio/kio_common.h"
#include "include/os_spec/kfs.h"

#include <stdlib.h>

#define KIFILE_BUFSIZE  (8192)

typedef struct tagKoFile {
  Ko ko;
  FILE* file;
  size_t default_bufsize;
} KoFile;

static KioFileOffset kofile_size(KoFile* kofile);
static void kofile_writer(KoFile* kofile);
static void kofile_close(KoFile* kofile);
static void kofile_detach(KoFile* kofile);

static void kofile_stdwriter(KoFile* kofile);  /* writer for stderr and stdout */

static const KoVirtualFunc kofile_create_vfunc = { .size = (KoSize)kofile_size, .delete = (KoDelete)kofile_close, .writer = (KoWriter)kofile_writer };
static const KoVirtualFunc kofile_attach_vfunc = { .size = (KoSize)kofile_size, .delete = (KoDelete)kofile_detach, .writer = (KoWriter)kofile_writer };
static const KoVirtualFunc kofile_stdstream_vfunc = { .size = (KoSize)kofile_size, .delete = (KoDelete)kofile_detach, .writer = (KoWriter)kofile_stdwriter };

Ko* kofile_create(const char* filepath, const char* mode) {
  FILE* file = fopen(filepath, mode);
  if (!file) return NULL;
  KoFile* kofile = (KoFile*)malloc(sizeof (KoFile));
  if (!kofile) {
    fclose(file);
    return NULL;
  }
  kofile->file = file;
  kofile->default_bufsize = KIFILE_BUFSIZE;
  ko_init((Ko*)kofile, &kofile_create_vfunc);
  return (Ko*)kofile;
}

Ko* kofile_attach(FILE* file) {
  KoFile* kofile = (KoFile*)malloc(sizeof (KoFile));
  if (!kofile) return NULL;
  kofile->file = file;
  kofile->default_bufsize = KIFILE_BUFSIZE;
  if (file == stderr || file == stdout) {
    ko_init((Ko*)kofile, &kofile_stdstream_vfunc);
  } else {
    ko_init((Ko*)kofile, &kofile_attach_vfunc);
  }
  return (Ko*)kofile;
}

static void kofile_writer(KoFile* kofile) {
  size_t writepos = ko_tell((Ko*)kofile) - ko_bufused((Ko*)kofile);
  if (fseek(kofile->file, writepos, SEEK_SET)) {
    ko_setbuf((Ko*)kofile, ko_getbuf((Ko*)kofile), 0, writepos);
    return;
  }
  void* buf = ko_getbuf((Ko*)kofile);
  if (!buf && !(buf = malloc(kofile->default_bufsize)))
      return;
  size_t writesize = fwrite(buf, 1, ko_bufused((Ko*)kofile), kofile->file);
  ko_setbuf((Ko*)kofile, buf, kofile->default_bufsize, writepos + writesize);
}

static void kofile_stdwriter(KoFile* kofile) {
  size_t writepos = ko_tell((Ko*)kofile) - ko_bufused((Ko*)kofile);
  void* buf = ko_getbuf((Ko*)kofile);
  if (!buf && !(buf = malloc(kofile->default_bufsize)))
      return;
  /* always write to tail */
  size_t writesize = fwrite(buf, 1, ko_bufused((Ko*)kofile), kofile->file);
  ko_setbuf((Ko*)kofile, buf, kofile->default_bufsize, writepos + writesize);
}

static void kofile_close(KoFile* kofile) {
  fclose(kofile->file);
  free(ko_getbuf((Ko*)kofile));
  free(kofile);
}

static void kofile_detach(KoFile* kofile) {
  free(ko_getbuf((Ko*)kofile));
  free(kofile);
}

static KioFileOffset kofile_size(KoFile* kofile) {
  return kfs_file_size(kofile->file);
}
