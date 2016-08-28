#include <system.h>

extern void initTXT(int n)
{

	bfont_set_encoding(n);

	dbglog(DBG_DEBUG, "\nText Data Initialized and Ready\n");

}

extern void initBG(char *bg)
{

	back_tex = pvr_mem_malloc(512 * 512 * 2);
	png_to_texture(bg, back_tex, PNG_NO_ALPHA);

	dbglog(DBG_DEBUG, "Background Initialized and Ready\n\n");

}

extern void drawBG()
{
	pvr_poly_cxt_t cxt;
	pvr_poly_hdr_t hdr;
	pvr_vertex_t vert;

	pvr_poly_cxt_txr(&cxt, PVR_LIST_OP_POLY, PVR_TXRFMT_RGB565, 512, 512, back_tex,
					 PVR_FILTER_NONE);
	pvr_poly_compile(&hdr, &cxt);
	pvr_prim(&hdr, sizeof(hdr));

	vert.argb = PVR_PACK_COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	vert.oargb = 0;
	vert.flags = PVR_CMD_VERTEX;

	vert.x = 1;
	vert.y = 1;
	vert.z = 1;
	vert.u = 0.0;
	vert.v = 0.0;
	pvr_prim(&vert, sizeof(vert));

	vert.x = DISP_WIDTH;
	vert.y = 1;
	vert.z = 1;
	vert.u = 1.0;
	vert.v = 0.0;
	pvr_prim(&vert, sizeof(vert));

	vert.x = 1;
	vert.y = DISP_HEIGHT;
	vert.z = 1;
	vert.u = 0.0;
	vert.v = 1.0;
	pvr_prim(&vert, sizeof(vert));

	vert.x = DISP_WIDTH;
	vert.y = DISP_HEIGHT;
	vert.z = 1;
	vert.u = 1.0;
	vert.v = 1.0;
	vert.flags = PVR_CMD_VERTEX_EOL;
	pvr_prim(&vert, sizeof(vert));
}

extern void initVideo(int disMode, int pixMode)
{
	vid_init(disMode, pixMode);
	pvr_init_defaults();
}

extern void clearVideo(int r, int g, int b)
{
	vid_clear(r, g, b);
	vid_flip(-1);
}

extern void drawHLine(int x1, int x2, int y, uint16 c)
{
	int cx;
	uint16 *line_buffer;

	// Swap x1 and x2 if necessary
	if (x1 > x2)
	{
		cx = x1;
		x1 = x2;
		x2 = cx;
	}

	// Don't bother if the line's off the screen
	if ((y < 0) || (y > (DISP_HEIGHT - 1)) || (x1 > (DISP_WIDTH - 1)) || (x2 < 0))
		return;

	// Clip the line to the edge of the screen
	if (x1 < 0)
		x1 = 0;
	if (x2 > (DISP_WIDTH - 1))
		x2 = (DISP_WIDTH - 1);

	// Set line_buffer to the beginning of the line
	line_buffer = vram_s + x1 + (y * DISP_WIDTH);

	// Draw the line
	for (cx = x1; cx <= x2; cx++)
		*line_buffer++ = c;
}

extern void drawVLine(int x, int y1, int y2, uint16 c)
{
	int cy;
	uint16 *line_buffer;

	// Swap y1 and y2 if necessary
	if (y1 > y2)
	{
		cy = y1;
		y1 = y2;
		y2 = cy;
	}

	// Don't bother if the line's off the screen
	if ((x < 0) || (x > (DISP_WIDTH - 1)) || (y1 > (DISP_HEIGHT - 1)) || (y2 < 0))
		return;

	// Clip the line to the edge of the screen
	if (y1 < 0)
		y1 = 0;
	if (y2 > (DISP_HEIGHT - 1))
		y2 = (DISP_HEIGHT - 1);

	// Set line_buffer to the beginning of the line
	line_buffer = vram_s + x + (y1 * DISP_WIDTH);

	// Draw the line
	for (cy = y1; cy <= y2; cy++)
	{
		*line_buffer = c;
		line_buffer += DISP_WIDTH;
	}
}

extern void drawCBox(int x1, int y1, int x2, int y2, uint16 c)
{
	drawHLine(x1, x2, y1, c);
	drawHLine(x1, x2, y2, c);
	drawVLine(x1, y1, y2, c);
	drawVLine(x2, y1, y2, c);
}

extern void drawFBox(int x1, int y1, int x2, int y2, uint16 c)
{
	int cy;
	for (cy = y1; cy <= y2; cy++)
		drawHLine(x1, x2, cy, c);
}

extern void printPVR(int x, int y, char *str)
{
	int strt = 24;
	int chrt = 12;
	bfont_draw_str(vram_s + (strt + y) * DISP_WIDTH + (chrt + x), DISP_WIDTH, 0, str);
}
