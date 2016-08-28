#include <system.h>

extern uint8 romdisk[];
KOS_INIT_FLAGS(INIT_DEFAULT | INIT_MALLOCSTATS);
KOS_INIT_ROMDISK(romdisk);

int bCount = 0;
int mountState = 0;
enum stateMachine stateM = stateBoot;

int main(int argc, char **argv)
{
	while (1)
	{
		switch (stateM)
		{

			case statePaused:
				printPVR((DISP_WIDTH / 2) - 36, (DISP_HEIGHT / 2) - 24, "Paused");
				break;

			case stateMenu:
				draw();
				break;

			case statePrintDIR:
				//printDIR("/hd");
				break;

			case stateBoot:
				printPVR(12, 24, "Initializing eCastOS...");
				initVideo(DM_640x480, PM_RGB565);
				initTXT(TEXT_ENC);
				initBG("/rd/bg.png");

				if (initHDD())
					dbglog(DBG_DEBUG, "* Failed to Initialize HDD!\n");
				if (initFS("/hd"))
					dbglog(DBG_DEBUG, "* Failed to Initialize EXT2 FS!\n");

				sleep(2);
				stateM = stateMenu;
				break;

			case stateSettings:
				settingDBRead("/hd/settings.conf");
				stateM = stateMenu;
				break;

		}
		update();
	}

	return 0;
}

void update()
{

	maple_device_t *cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);

	if (cont)
	{
		cont_state_t *state = (cont_state_t *) maple_dev_status(cont);

		if (!state)
			return;

		if (state->buttons & CONT_DPAD_UP || state->buttons & CONT_DPAD2_UP)
		{

			//dbglog(DBG_DEBUG, "\nDPad UP Pressed\n");

			if (stateM == statePaused)
			{
				dbglog(DBG_DEBUG, "\n*** RESUME RENDERING PVR ***\n");
				stateM = stateMenu;
			}
			else if (stateM != statePaused)
			{
				dbglog(DBG_DEBUG, "\n*** PAUSE RENDERING PVR ***\n");
				stateM = statePaused;
			}

			usleep(400000);

		}

		if (state->buttons & CONT_DPAD_DOWN || state->buttons & CONT_DPAD2_DOWN)
		{

			if (stateM != stateSettings)
			{
				stateM = stateSettings;
				dbglog(DBG_DEBUG, "~ Settings ~\n");
			}

			usleep(400000);
		}

		if (state->buttons & CONT_DPAD_LEFT || state->buttons & CONT_DPAD2_LEFT)
		{


		}

		if (state->buttons & CONT_DPAD_RIGHT || state->buttons & CONT_DPAD2_RIGHT)
		{


		}

		if (state->buttons & CONT_Y)
		{

			//dbglog(DBG_DEBUG, "\nButton Y Pressed\n");

			if (writeRTF())
				dbglog(DBG_DEBUG, "** FS WRITE FAILED!\n");

			if (settingDBSave("/hd/settings.conf", "CYRUS", "HelloWorld", 1))
				dbglog(DBG_DEBUG, "** Settings Save Failed!\n");

			usleep(400000);

		}

		if (state->buttons & CONT_A)
		{

			dbglog(DBG_DEBUG, "\nButton A Pressed\n");

			//if (executeSub("/rd/SUB.ECS"))
			//	dbglog(DBG_DEBUG, "** EXEC FAILED!\n");
			usleep(400000);

		}

		if (state->buttons & CONT_X)
		{

			//dbglog(DBG_DEBUG, "\nButton X Pressed\n");
			if (mountState)
			{
				mountCTL("/hd", 0);
				mountState = 0;
			}
			else if (!mountState)
			{
				mountCTL("/hd", 1);
				mountState = 1;
			}

			usleep(400000);

		}

		if (state->buttons & CONT_B)
		{

			//dbglog(DBG_DEBUG, "\nButton B Pressed\n");

			bCount += 1;
			stateM = statePrintDIR;

			if (bCount == 2)
			{
				stateM = stateMenu;
				bCount = 0;
				dbglog(DBG_DEBUG, "~ STATEMACHINE set to Menu\n");
			}
			else if ((stateM == statePrintDIR) && (mountState))
			{
				dbglog(DBG_DEBUG, "~ STATEMACHINE set to PrintDIR\n");
				printDIR("/hd");
			}

			usleep(400000);

		}

		if (state->buttons & CONT_START)
		{

			dbglog(DBG_DEBUG, "\nStart Button Pressed\n\n");
			shutDown("/hd");

		}
	}
}

// Main menu
void draw()
{
	pvr_wait_ready();
	pvr_scene_begin();
	pvr_list_begin(PVR_LIST_OP_POLY);

	if (stateM != statePaused)
		drawBG();

	pvr_list_finish();

	pvr_list_begin(PVR_LIST_TR_POLY);
	printPVR(0, 0, "eCastOS v0.3.3 | EXT2");

	if (stateM != statePaused)
	{

		printPVR(0, 48, "A  : Boot RomDisk Binary --disabled");
		printPVR(0, 72, "B  : List Root Directory");

		if (mountState)
			printPVR(0, 96, "X  : Unmount G1");

		if (!mountState)
		{
			printPVR(0, 96, "X  : Mount G1");
			printPVR(0, 216, "G1 : Not Mounted");
		}

		printPVR(0, 120, "Y     : Write HardDrive File's");
		printPVR(0, 168, "UP    : Pause Rendering PowerVR");
		printPVR(0, 192, "DOWN  : Check Settings Under DBG");

	}

	pvr_list_finish();
	pvr_scene_finish();
}
