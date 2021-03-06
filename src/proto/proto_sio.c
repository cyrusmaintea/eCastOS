#include <system.h>

extern int initSIO(int baud, int fifo)
{
	scif_set_parameters(baud, fifo);
	if (!scif_detected())
	{
		dbglog(DBG_DEBUG, "* SCIF Device NOT Detected!\n");
		return 1;
	}
	if (!scif_init())
	{
		dbglog(DBG_DEBUG, "* SCIF Device Failed to Initialize!\n");
		return 1;
	}
	return 0;
}

extern int stopSIO()
{
	if (!scif_shutdown())
	{
		dbglog(DBG_DEBUG, "* SCIF Device Failed to Stop!\n");
		return 1;
	}
	return 0;
}

extern int flushSIO()
{
	if (!scif_flush())
	{
		dbglog(DBG_DEBUG, "~ Flushed SCIF\n");
	}
	else
	{
		dbglog(DBG_DEBUG, "* FAILED Flush SCIF\n");
		return 1;
	}
	return 0;
}
