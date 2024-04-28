CC = gcc
CFLAGS = -O2 -I $(ROOT_DIR)
AR = ar rcs



ROOT_DIR = $(abspath .)/

INC_DIR = $(ROOT_DIR)include/
SRC_DIR = $(ROOT_DIR)src/
LIB_DIR = $(ROOT_DIR)lib/

HMAP_INC_DIR = $(INC_DIR)hashmap/
HMAP_DIR = $(SRC_DIR)hashmap/
SET_INC_DIR = $(INC_DIR)set/
SET_DIR = $(SRC_DIR)set/
ARRAY_INC_DIR = $(INC_DIR)array/
ARRAY_DIR = $(SRC_DIR)array/
QUEUE_INC_DIR = $(INC_DIR)queue/
QUEUE_DIR = $(SRC_DIR)queue/
OS_SPEC_DIR = $(SRC_DIR)os_spec/
OS_SPEC_INC_DIR = $(INC_DIR)os_spec/
STR_DIR = $(SRC_DIR)string/
STR_INC_DIR = $(INC_DIR)string/
KIO_DIR = $(SRC_DIR)kio/
KIO_INC_DIR = $(INC_DIR)kio/

OBJS = $(LIB_DIR)dir.o $(LIB_DIR)file.o $(LIB_DIR)setint_map.o $(LIB_DIR)strx_map.o \
       $(LIB_DIR)kmap.o $(LIB_DIR)bitset.o $(LIB_DIR)hashset.o $(LIB_DIR)kiqueue.o \
       $(LIB_DIR)kqueue.o $(LIB_DIR)karray.o $(LIB_DIR)ko.o $(LIB_DIR)ki.o  $(LIB_DIR)kobuf.o \
       $(LIB_DIR)kibuf.o $(LIB_DIR)kofile.o $(LIB_DIR)kifile.o $(LIB_DIR)ko_printf.o


$(LIB_DIR)libk.a : $(OBJS) | create_lib_dir
	$(AR) $@ $^


$(LIB_DIR)dir.o : $(OS_SPEC_DIR)dir.c $(OS_SPEC_INC_DIR)dir.h | create_lib_dir
	$(CC) -c -o $@ $< $(CFLAGS) 

$(LIB_DIR)file.o : $(OS_SPEC_DIR)file.c $(OS_SPEC_INC_DIR)file.h | create_lib_dir
	$(CC) -c -o $@ $< $(CFLAGS) 

$(LIB_DIR)kmap.o : $(HMAP_DIR)kmap.c $(HMAP_INC_DIR)kmap.h | create_lib_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB_DIR)intset_map.o : $(HMAP_DIR)intset_map.c $(HMAP_INC_DIR)intset_map.h $(SET_INC_DIR)bitset.h | create_lib_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB_DIR)setint_map.o : $(HMAP_DIR)setint_map.c $(HMAP_INC_DIR)setint_map.h $(SET_INC_DIR)bitset.h | create_lib_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB_DIR)str_map.o : $(HMAP_DIR)str_map.c $(HMAP_INC_DIR)str_map.h | create_lib_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB_DIR)strx_map.o : $(HMAP_DIR)strx_map.c $(HMAP_INC_DIR)strx_map.h | create_lib_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB_DIR)bitset.o : $(SET_DIR)bitset.c $(SET_INC_DIR)bitset.h | create_lib_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB_DIR)hashset.o : $(SET_DIR)hashset.c $(SET_INC_DIR)hashset.h | create_lib_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB_DIR)karray.o : $(ARRAY_DIR)karray.c $(ARRAY_INC_DIR)karray.h | create_lib_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB_DIR)kiqueue.o : $(QUEUE_DIR)kiqueue.c $(QUEUE_INC_DIR)kiqueue.h | create_lib_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB_DIR)kqueue.o : $(QUEUE_DIR)kqueue.c $(QUEUE_INC_DIR)kqueue.h | create_lib_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB_DIR)plugin.o : $(OS_SPEC_DIR)plugin.c $(OS_SPEC_INC_DIR)plugin.h | create_lib_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB_DIR)ki.o : $(KIO_DIR)ki.c $(KIO_INC_DIR)ki.h | create_lib_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB_DIR)ko.o : $(KIO_DIR)ko.c $(KIO_INC_DIR)ko.h | create_lib_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB_DIR)kibuf.o : $(KIO_DIR)kibuf.c $(KIO_INC_DIR)kibuf.h | create_lib_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB_DIR)kobuf.o : $(KIO_DIR)kobuf.c $(KIO_INC_DIR)kobuf.h | create_lib_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB_DIR)kifile.o : $(KIO_DIR)kifile.c $(KIO_INC_DIR)kifile.h | create_lib_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB_DIR)kofile.o : $(KIO_DIR)kofile.c $(KIO_INC_DIR)kofile.h | create_lib_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIB_DIR)ko_printf.o : $(KIO_DIR)ko_printf.c $(KIO_INC_DIR)kio.h | create_lib_dir
	$(CC) -c -o $@ $< $(CFLAGS)


.PHONY: clean create_lib_dir
clean :
	$(RM) $(LIB_DIR)*

create_lib_dir :
	mkdir -p $(LIB_DIR)
