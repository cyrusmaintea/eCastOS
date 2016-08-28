#include <system.h>
#include <subelf.h>

extern uint8 romdisk[];
KOS_INIT_FLAGS(INIT_DEFAULT | INIT_MALLOCSTATS);
KOS_INIT_ROMDISK(romdisk);

int main(int argc, char **argv) {

	initVideo(DM_640x480, PM_RGB565);

	initTXT(TEXT_ENC);

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

			dbglog(DBG_DEBUG, "\nStart Button Pressed\n\n");
			arch_exit();
		}

		if(state->buttons & CONT_B) {
			dbglog(DBG_DEBUG, "\nButton B Pressed\n");
		}

	}

}

void drawSub() {

    pvr_wait_ready();
    pvr_scene_begin();

	pvr_list_begin(PVR_LIST_OP_POLY);

	pvr_list_finish();

	pvr_list_begin(PVR_LIST_TR_POLY);

	printPVR(20, 20, "eCastos v0.3.2");
	printPVR(20, 45, "Welcome to Subelf :D");
	printPVR(20, 70, "Press START To END");

	pvr_list_finish();

    pvr_scene_finish();

}
