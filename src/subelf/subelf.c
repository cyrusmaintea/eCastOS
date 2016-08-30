#include <system.h>
#include <subelf.h>

extern uint8 romdisk[];
KOS_INIT_FLAGS(INIT_DEFAULT | INIT_MALLOCSTATS);
KOS_INIT_ROMDISK(romdisk);

int main(int argc, char **argv) {

	pvr_init_defaults();
	initTXT(TEXT_ENC);

	initBG("/rd/bg.png");


	while (1) {
		updateSub();
		drawSub();

	}
    return 0;
}

void updateSub() {
	maple_device_t *cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
    if(cont) {
        cont_state_t *state = (cont_state_t *)maple_dev_status(cont);

        if(!state)
            return;
		if(state->buttons & CONT_START) {

			dbglog(DBG_DEBUG, "\nStart Pressed\n\n");
			arch_exit();
		}
		if(state->buttons & CONT_B) {
			//dbglog(DBG_DEBUG, "\nButton B Pressed\n");
		}
	}
}

void drawSub() {
    pvr_wait_ready();
    pvr_scene_begin();

	pvr_list_begin(PVR_LIST_OP_POLY);
		drawBG();
	pvr_list_finish();

	pvr_list_begin(PVR_LIST_TR_POLY);

	printPVR(12, 24, "eCastos 0.3.6");
	printPVR(12, 48, "Welcome to Subelf!");
	printPVR(12, 72, "Press START To END");

	pvr_list_finish();

    pvr_scene_finish();
}

void display() {
	drawCBox(24, 256, 616, 256, PACK_PIXEL(128, 128, 128));
}
