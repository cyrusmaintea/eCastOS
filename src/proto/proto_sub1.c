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
	pvr_poly_cxt_txr(&cxt, PVR_LIST_OP_POLY, PVR_TXRFMT_RGB565, 512, 512, back_tex, PVR_FILTER_BILINEAR);
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

extern void clearVideo(int r, int g, int b)
{
	vid_clear(r, g, b);
	vid_flip(-1);
}

extern void printPVR(int x, int y, char *str)
{
	int strt = 24;
	int chrt = 12;
	bfont_draw_str(vram_s + (strt + y) * DISP_WIDTH + (chrt + x), DISP_WIDTH, 0, str);
}
