BUILD = build
SRC = src

INCLUDES = -Iinclude
LIBS = -lkosext2fs -lpng -lz -lm
LIBRARYS = lib/libsupport.a lib/libproto.a

TARGET = $(BUILD)/main.elf
BINTARGET = $(BUILD)/1ST_READ.BIN

OBJS = $(BUILD)/main.o $(BUILD)/proto_sub1.o $(BUILD)/proto_sub2.o $(BUILD)/proto_sio.o romdisk.o

KOS_ROMDISK_DIR = romdisk

SUBTARGET = $(BUILD)/subelf.elf
SUBOBJS =  $(BUILD)/proto_sub1.o $(BUILD)/proto_sub2.o romdisk.o $(BUILD)/subelf.o

include $(KOS_BASE)/Makefile.rules

rebuild:
	make clean
	make all

all: buildlib build

buildlib: supportlib protolib

build: buildsub genromfs buildboot

dbg:
	$(KOS_LOADER) -b 1500000 -t COM2 -x $(TARGET)

run:
	$(KOS_LOADER) -b 1500000 -t COM2 -x $(BINTARGET)

clean:
	-rm -f $(TARGET) $(BINTARGET) romdisk/SUB.ECS $(SUBTARGET) $(LIBRARYS) $(OBJS) $(SUBOBJS) romdisk.* $(BUILD)/*.o

buildboot: main.o $(TARGET)

buildsub: subelf.o $(SUBTARGET)

supportlib: enc28j60.o #g2_ide.o
	$(KOS_AR) rcs lib/libsupport.a $(BUILD)/enc28j60.o #$(BUILD)/g2_ide.o

protolib: proto_sio.o proto_sub1.o proto_sub2.o
	$(KOS_AR) rcs lib/libproto.a $(BUILD)/proto_sio.o $(BUILD)/proto_sub1.o $(BUILD)/proto_sub2.o

main.o: $(SRC)/main.c
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) $(INCLUDES) -c $(SRC)/main.c -o $(BUILD)/main.o

subelf.o: $(SRC)/subelf/subelf.c
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) $(INCLUDES) -c $(SRC)/subelf/subelf.c -o $(BUILD)/subelf.o

enc28j60.o: $(SRC)/support/enc28j60.c
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) $(INCLUDES) -c $(SRC)/support/enc28j60.c -o $(BUILD)/enc28j60.o

#g2_ide.o: $(SRC)/support/g2_ide.c
#	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) $(INCLUDES) -c $(SRC)/support/g2_ide.c -o $(BUILD)/g2_ide.o

proto_sio.o: $(SRC)/proto/proto_sio.c
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) $(INCLUDES) -c $(SRC)/proto/proto_sio.c -o $(BUILD)/proto_sio.o

proto_sub1.o: $(SRC)/proto/proto_sub1.c
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) $(INCLUDES) -c $(SRC)/proto/proto_sub1.c -o $(BUILD)/proto_sub1.o

proto_sub2.o: $(SRC)/proto/proto_sub2.c
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) $(INCLUDES) -c $(SRC)/proto/proto_sub2.c -o $(BUILD)/proto_sub2.o

genromfs:
	genromfs.exe -f romdisk.img -d romdisk -v -x .git .svn
	$(KOS_BASE)/utils/bin2o/bin2o romdisk.img romdisk romdisk.o

$(TARGET): $(OBJS)
	$(KOS_CC) $(LIBRARYS) $(KOS_CFLAGS) $(KOS_LDFLAGS) -o $@ $(KOS_START) $(OBJS) $(DATAOBJS) $(OBJEXTRA) $(LIBS) $(KOS_LIBS)
	$(KOS_OBJCOPY) -O binary -R .stack $@ build/1ST_READ.BIN

$(SUBTARGET): $(SUBOBJS)
	$(KOS_CC) $(LIBRARYS) $(KOS_CFLAGS) $(KOS_LDFLAGS) -o $@ $(KOS_START) $(SUBOBJS) $(DATAOBJS) $(OBJEXTRA) $(LIBS) $(KOS_LIBS)
	$(KOS_STRIP) $(BUILD)/subelf.elf
	$(KOS_OBJCOPY) -O binary -R .stack $(BUILD)/subelf.elf $(BUILD)/SUB.ECS
	cp $(BUILD)/SUB.ECS romdisk/SUB.ECS
