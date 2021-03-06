BUILD = build
SRC = src

INCLUDES = -Iinclude
LIBS = -lkosext2fs -lpng -lz -lm
LIBRARYS = lib/libsupport.a lib/libproto.a

TARGET = $(BUILD)/main.elf
BINTARGET = $(BUILD)/1ST_READ.BIN

OBJS = $(BUILD)/main.o $(BUILD)/proto_sub1.o $(BUILD)/proto_sub2.o $(BUILD)/proto_sio.o $(BUILD)/proto_util.o romdisk.o

KOS_ROMDISK_DIR = romdisk

SUBTARGET = $(BUILD)/subelf.elf
SUBOBJS = $(BUILD)/proto_util.o $(BUILD)/proto_sio.o $(BUILD)/proto_sub1.o $(BUILD)/proto_sub2.o $(BUILD)/subelf.o romdisk.o

include $(KOS_BASE)/Makefile.rules

rebuild:
	make clean
	make all

all: buildlib build

buildlib: supportlib protolib

build: buildsub genromfs buildboot

dbg:
	$(KOS_LOADER) -b 1500000 -t COM5 -x $(TARGET)

run:
	$(KOS_LOADER) -b 1500000 -t COM5 -x $(BINTARGET)

subdbg:
	$(KOS_LOADER) -b 1500000 -t COM5 -x $(SUBTARGET)

clean:
	rm -f romdisk/SUB.ECS romdisk.* $(BUILD)/*

buildboot: main.o $(TARGET)

buildsub: subelf.o $(SUBTARGET)

supportlib: enc28j60.o
	$(KOS_AR) rcs lib/libsupport.a $(BUILD)/enc28j60.o

protolib: proto_sio.o proto_sub1.o proto_sub2.o proto_util.o
	$(KOS_AR) rcs lib/libproto.a $(BUILD)/proto_sio.o $(BUILD)/proto_sub1.o $(BUILD)/proto_sub2.o $(BUILD)/proto_util.o

main.o: $(SRC)/main.c
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) $(INCLUDES) -c $(SRC)/main.c -o $(BUILD)/main.o

subelf.o: $(SRC)/subelf/subelf.c
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) $(INCLUDES) -c $(SRC)/subelf/subelf.c -o $(BUILD)/subelf.o

enc28j60.o: $(SRC)/support/enc28j60.c
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) $(INCLUDES) -c $(SRC)/support/enc28j60.c -o $(BUILD)/enc28j60.o

proto_sio.o: $(SRC)/proto/proto_sio.c
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) $(INCLUDES) -c $(SRC)/proto/proto_sio.c -o $(BUILD)/proto_sio.o

proto_sub1.o: $(SRC)/proto/proto_sub1.c
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) $(INCLUDES) -c $(SRC)/proto/proto_sub1.c -o $(BUILD)/proto_sub1.o

proto_sub2.o: $(SRC)/proto/proto_sub2.c
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) $(INCLUDES) -c $(SRC)/proto/proto_sub2.c -o $(BUILD)/proto_sub2.o

proto_util.o: $(SRC)/proto/proto_util.c
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) $(INCLUDES) -c $(SRC)/proto/proto_util.c -o $(BUILD)/proto_util.o

genromfs:
	genromfs.exe -f romdisk.img -d romdisk -v -x .git .svn
	$(KOS_BASE)/utils/bin2o/bin2o romdisk.img romdisk romdisk.o

$(TARGET): $(OBJS)
	$(KOS_CC) $(LIBRARYS) $(KOS_CFLAGS) $(KOS_LDFLAGS) $(INCLUDES) -o $@ $(KOS_START) $(OBJS) $(DATAOBJS) $(OBJEXTRA) $(LIBS) $(KOS_LIBS)
	$(KOS_STRIP) $(BUILD)/main.elf
	$(KOS_OBJCOPY) -O binary -R .stack $@ $(BINTARGET)

$(SUBTARGET): $(SUBOBJS)
	$(KOS_CC) $(LIBRARYS) $(KOS_CFLAGS) $(KOS_LDFLAGS) $(INCLUDES) -o $@ $(KOS_START) $(SUBOBJS) $(DATAOBJS) $(OBJEXTRA) $(LIBS) $(KOS_LIBS)
	$(KOS_STRIP) $(BUILD)/subelf.elf
	$(KOS_OBJCOPY) -O binary -R .stack $@ $(BUILD)/SUB.ECS
	cp $(BUILD)/SUB.ECS romdisk/SUB.ECS
