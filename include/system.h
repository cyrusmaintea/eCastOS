//*****************************************************************//
//KOS
#include <kos.h>
//LUA
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
//POR
#include <png/png.h>
#include <zlib/zlib.h>
#include <ext2/fs_ext2.h>
//STD
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <dirent.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
//DC
#include <dc/g1ata.h>
#include <dc/maple/keyboard.h>
//ARC
#include <arch/timer.h>
#include <arch/types.h>
//LOC
#include <enc28j60.h>
#include <proto_sub1.h>
#include <proto_sub2.h>
#include <proto_sio.h>
#include <proto_util.h>
//*****************************************************************//

//DEFINES
#define TEXT_ENC				0
#define BUSMODE					0
#define MNT_MODE 				FS_EXT2_MOUNT_READWRITE
#define DISP_WIDTH				640
#define DISP_HEIGHT				480

enum stateMachine {
	stateBoot,
	statePrintDIR,
	statePaused,
	stateSettings,
	stateMenu
};

//VARS
static unsigned char piobuf[] __attribute__((aligned(32)));
static unsigned char tmp[] __attribute__((aligned(32)));
pvr_ptr_t back_tex;
kos_blockdev_t bd_pio;
uint8 pt;
FILE *fp;
file_t f;
int mountState;
struct dirent *entry;

//settings
char user[32];
char motd[128];
int id;

//LUA
lua_State *L1;
lua_State *L2;

//Author's methods Start:
void update();
void core();
