CC = gcc
OPTIMIZE = -O2
MEMORY_CHECK =
WARNING = -Wall -Wextra -Winline
DEBUG = -DNDEBUG

CFLAGS = $(OPTIMIZE) $(MEMORY_CHECK) $(WARNING) $(DEBUG) -I $(ROOT_DIR)

ROOT_DIR = $(abspath .)/

INC_DIR = $(ROOT_DIR)include/
SRC_DIR = $(ROOT_DIR)src/
LIB_DIR = $(ROOT_DIR)lib/
OBJ_DIR := $(ROOT_DIR)obj/


OBJS = $(OBJ_DIR)setint_map.o $(OBJ_DIR)strx_map.o $(OBJ_DIR)kmap.o $(OBJ_DIR)bitset.o \
       $(OBJ_DIR)hashset.o $(OBJ_DIR)kiqueue.o $(OBJ_DIR)kqueue.o $(OBJ_DIR)karray.o \
       $(OBJ_DIR)ko.o $(OBJ_DIR)ki.o  $(OBJ_DIR)kobuf.o $(OBJ_DIR)kibuf.o $(OBJ_DIR)kofile.o \
       $(OBJ_DIR)kifile.o $(OBJ_DIR)ko_printf.o	$(OBJ_DIR)lib.o $(OBJ_DIR)kfs.o

PIC_OBJS = $(patsubst %.o, %.pic.o, $(OBJS))


all : $(LIB_DIR)libk.a $(LIB_DIR)libk.so


$(LIB_DIR)libk.a : $(OBJS) | create_dir
	$(AR) $@ $^

$(LIB_DIR)libk.so : $(PIC_OBJS) | create_dir
	$(CC) -shared -o $@ $^


$(OBJ_DIR)karray.o : $(SRC_DIR)array/karray.c $(INC_DIR)array/karray.h $(INC_DIR)kutils/utils.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR)kmap.o : $(SRC_DIR)hashmap/kmap.c $(INC_DIR)hashmap/kmap.h $(INC_DIR)kutils/utils.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR)setint_map.o : $(SRC_DIR)hashmap/setint_map.c $(INC_DIR)hashmap/setint_map.h $(INC_DIR)set/bitset.h $(INC_DIR)bits/bitop.h $(INC_DIR)kutils/utils.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR)strx_map.o : $(SRC_DIR)hashmap/strx_map.c $(INC_DIR)hashmap/strx_map.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR)kibuf.o : $(SRC_DIR)kio/kibuf.c $(INC_DIR)kio/kibuf.h $(INC_DIR)kio/ki.h $(INC_DIR)kio/kio_common.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR)ki.o : $(SRC_DIR)kio/ki.c $(INC_DIR)kio/ki.h $(INC_DIR)kio/kio_common.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR)kifile.o : $(SRC_DIR)kio/kifile.c $(INC_DIR)kio/kifile.h $(INC_DIR)kio/ki.h $(INC_DIR)kio/kio_common.h $(INC_DIR)os_spec/kfs.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR)kobuf.o : $(SRC_DIR)kio/kobuf.c $(INC_DIR)kio/kobuf.h $(INC_DIR)kio/ko.h $(INC_DIR)kio/kio_common.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR)ko.o : $(SRC_DIR)kio/ko.c $(INC_DIR)kio/ko.h $(INC_DIR)kio/kio_common.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR)kofile.o : $(SRC_DIR)kio/kofile.c $(INC_DIR)kio/kofile.h $(INC_DIR)kio/ko.h $(INC_DIR)kio/kio_common.h $(INC_DIR)os_spec/kfs.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR)ko_printf.o : $(SRC_DIR)kio/ko_printf.c $(INC_DIR)kio/ko.h $(INC_DIR)kio/kio_common.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR)lib.o : $(SRC_DIR)lib/lib.c $(INC_DIR)lib/lib.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR)kfs.o : $(SRC_DIR)os_spec/kfs.c $(INC_DIR)os_spec/kfs.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR)kiqueue.o : $(SRC_DIR)queue/kiqueue.c $(INC_DIR)queue/kiqueue.h $(INC_DIR)kutils/utils.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR)kqueue.o : $(SRC_DIR)queue/kqueue.c $(INC_DIR)queue/kqueue.h $(INC_DIR)kutils/utils.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR)bitset.o : $(SRC_DIR)set/bitset.c $(INC_DIR)set/bitset.h $(INC_DIR)bits/bitop.h $(INC_DIR)kutils/utils.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR)hashset.o : $(SRC_DIR)set/hashset.c $(INC_DIR)set/hashset.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS)

# ==========================SHARED OBJECT=============================
$(OBJ_DIR)karray.pic.o : $(SRC_DIR)array/karray.c $(INC_DIR)array/karray.h $(INC_DIR)kutils/utils.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS) -fPIC

$(OBJ_DIR)kmap.pic.o : $(SRC_DIR)hashmap/kmap.c $(INC_DIR)hashmap/kmap.h $(INC_DIR)kutils/utils.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS) -fPIC

$(OBJ_DIR)setint_map.pic.o : $(SRC_DIR)hashmap/setint_map.c $(INC_DIR)hashmap/setint_map.h $(INC_DIR)set/bitset.h $(INC_DIR)bits/bitop.h $(INC_DIR)kutils/utils.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS) -fPIC

$(OBJ_DIR)strx_map.pic.o : $(SRC_DIR)hashmap/strx_map.c $(INC_DIR)hashmap/strx_map.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS) -fPIC

$(OBJ_DIR)kibuf.pic.o : $(SRC_DIR)kio/kibuf.c $(INC_DIR)kio/kibuf.h $(INC_DIR)kio/ki.h $(INC_DIR)kio/kio_common.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS) -fPIC

$(OBJ_DIR)ki.pic.o : $(SRC_DIR)kio/ki.c $(INC_DIR)kio/ki.h $(INC_DIR)kio/kio_common.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS) -fPIC

$(OBJ_DIR)kifile.pic.o : $(SRC_DIR)kio/kifile.c $(INC_DIR)kio/kifile.h $(INC_DIR)kio/ki.h $(INC_DIR)kio/kio_common.h $(INC_DIR)os_spec/kfs.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS) -fPIC

$(OBJ_DIR)kobuf.pic.o : $(SRC_DIR)kio/kobuf.c $(INC_DIR)kio/kobuf.h $(INC_DIR)kio/ko.h $(INC_DIR)kio/kio_common.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS) -fPIC

$(OBJ_DIR)ko.pic.o : $(SRC_DIR)kio/ko.c $(INC_DIR)kio/ko.h $(INC_DIR)kio/kio_common.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS) -fPIC

$(OBJ_DIR)kofile.pic.o : $(SRC_DIR)kio/kofile.c $(INC_DIR)kio/kofile.h $(INC_DIR)kio/ko.h $(INC_DIR)kio/kio_common.h $(INC_DIR)os_spec/kfs.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS) -fPIC

$(OBJ_DIR)ko_printf.pic.o : $(SRC_DIR)kio/ko_printf.c $(INC_DIR)kio/ko.h $(INC_DIR)kio/kio_common.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS) -fPIC

$(OBJ_DIR)lib.pic.o : $(SRC_DIR)lib/lib.c $(INC_DIR)lib/lib.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS) -fPIC

$(OBJ_DIR)kfs.pic.o : $(SRC_DIR)os_spec/kfs.c $(INC_DIR)os_spec/kfs.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS) -fPIC

$(OBJ_DIR)kiqueue.pic.o : $(SRC_DIR)queue/kiqueue.c $(INC_DIR)queue/kiqueue.h $(INC_DIR)kutils/utils.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS) -fPIC

$(OBJ_DIR)kqueue.pic.o : $(SRC_DIR)queue/kqueue.c $(INC_DIR)queue/kqueue.h $(INC_DIR)kutils/utils.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS) -fPIC

$(OBJ_DIR)bitset.pic.o : $(SRC_DIR)set/bitset.c $(INC_DIR)set/bitset.h $(INC_DIR)bits/bitop.h $(INC_DIR)kutils/utils.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS) -fPIC

$(OBJ_DIR)hashset.pic.o : $(SRC_DIR)set/hashset.c $(INC_DIR)set/hashset.h | create_dir
	$(CC) -c -o $@ $< $(CFLAGS) -fPIC




.PHONY: clean create_dir all
clean :
	$(RM) -r $(OBJ_DIR)* $(LIB_DIR)*

create_dir :
	mkdir -p $(OBJ_DIR) $(LIB_DIR)
