/*
	DISPLAY MODES:
		DM_320x240_VGA : 320x240 VGA 60Hz
		DM_320x240_NTSC : 320x240 NTSC 60Hz
		DM_640x480_VGA : 640x480 VGA 60Hz
		DM_640x480_NTSC_IL : 640x480 NTSC Interlaced 60Hz
		DM_800x608_VGA : 800x608 VGA 60Hz
		DM_640x480_PAL_IL : 640x480 PAL Interlaced 50Hz
		DM_256x256_PAL_IL : 256x256 PAL Interlaced 50Hz
		DM_768x480_NTSC_IL : 768x480 NTSC Interlaced 60Hz
		DM_768x576_PAL_IL : 768x576 PAL Interlaced 50Hz
		DM_768x480_PAL_IL : 768x480 PAL Interlaced 50Hz
		DM_320x240_PAL : 320x240 PAL 50Hz
		DM_320x240_VGA_MB : 320x240 VGA 60Hz, 4FBs
		DM_320x240_NTSC_MB : 320x240 NTSC 60Hz, 4FBs
		DM_640x480_VGA_MB : 640x480 VGA 60Hz, 4FBs
		DM_640x480_NTSC_IL_MB : 640x480 NTSC IL 60Hz, 4FBs
		DM_800x608_VGA_MB : 800x608 VGA 60Hz, 4FBs
		DM_640x480_PAL_IL_MB : 640x480 PAL IL 50Hz, 4FBs
		DM_256x256_PAL_IL_MB : 256x256 PAL IL 50Hz, 4FBs
		DM_768x480_NTSC_IL_MB : 768x480 NTSC IL 60Hz, 4FBs
		DM_768x576_PAL_IL_MB : 768x576 PAL IL 50Hz, 4FBs
		DM_768x480_PAL_IL_MB : 768x480 PAL IL 50Hz, 4FBs
		DM_320x240_PAL_MB : 320x240 PAL 50Hz, 4FBs
*/

/*
	PIXEL MODES:
		PM_RGB555 : 15-bit (xRRRRRGGGGGBBBBB)
		PM_RGB565 : 16-bit (RRRRRGGGGGGBBBBB)
		PM_RGB888 : 24-bit (RRRRRRRR GGGGGGGG BBBBBBBB)
*/

#define TEXT_WESTERN BFONT_CODE_ISO8859_1
#define PACK_PIXEL(r, g, b) ( ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3) )
#define DRAW_PIXEL(x, y, c) \
   if((x >= 0) && (x < DISP_WIDTH) && (y >= 0) && (y < DISP_HEIGHT)) \
      vram_s[x + (y * DISP_WIDTH)] = c;
extern void initTXT(int n);
extern void initBG(char *bg);
extern void drawBG();
extern void clearVideo(int r, int g, int b);
extern void drawHLine(int x1, int x2, int y, uint16 c);
extern void drawVLine(int x, int y1, int y2, uint16 c);
extern void drawCBox(int x1, int y1, int x2, int y2, uint16 c);
extern void drawFBox(int x1, int y1, int x2, int y2, uint16 c);
extern void printPVR(int x, int y, char *str);
