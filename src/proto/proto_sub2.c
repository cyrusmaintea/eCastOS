#include <system.h>

extern void mountCTL(char *blkhdd, int state)
{
	if (state)
	{
		dbglog(DBG_DEBUG, "\n~ Mount State Was Set To True!\n");
		initFS(blkhdd);
	}
	else if (!state)
	{
		dbglog(DBG_DEBUG, "\n~ Mount State Was Set To False!\n");
		removeFS(blkhdd);
	}
}

extern void printDIR(char *dir)
{
	DIR *d;
	int chk = 0;
	// char count * 12 / 2
	int x = 216;

	dbglog(DBG_DEBUG, "\nListing the contents of %s:\n", dir);
	if (!(d = opendir(dir)))
	{
		dbglog(DBG_DEBUG, "Could not open %s: %s\n", dir, strerror(errno));
	}

	while ((entry = readdir(d)))
	{
		printPVR(((DISP_WIDTH / 2) - x), 0, "  Press B To Return");
		printPVR((DISP_WIDTH / 2) - 96, (45 + chk), entry->d_name);
		dbglog(DBG_DEBUG, "%s\n", entry->d_name);
		chk += 25;
	}
	chk = 0;
	if (closedir(d))
		dbglog(DBG_DEBUG, "Could not close directory: %s\n", strerror(errno));
}

extern void printRFS()
{
	DIR *d;

	dbglog(DBG_DEBUG, "Listing the contents of root:\n");
	if (!(d = opendir("/hd")))
	{
		dbglog(DBG_DEBUG, "Could not open /hd: %s\n", strerror(errno));
	}

	while ((entry = readdir(d)))
		dbglog(DBG_DEBUG, "%s\n", entry->d_name);

	if (closedir(d))
	{
		dbglog(DBG_DEBUG, "Could not close directory: %s\n", strerror(errno));
	}
}

extern int checkHDD()
{
	if (g1_ata_blockdev_for_partition(0, 0, &bd_pio, &pt))
	{
		dbglog(DBG_DEBUG, "* Couldn't get PIO blockdev for partition!\n");
		return -1;
	}
	dbglog(DBG_DEBUG, "~ ATA Device Hardware Check Passed\n");
	return 0;
}

extern int initHDD()
{
	if (g1_ata_init())
	{
		dbglog(DBG_DEBUG, "G1ATA Failed to Initialize!\n");
		return -1;
	}
	if (g1_ata_select_device(G1_ATA_SLAVE))
	{
		dbglog(DBG_DEBUG, "G1ATA Failed to set SLAVE!\n");
		return -1;
	}
	if (g1_ata_blockdev_for_partition(0, 0, &bd_pio, &pt))
	{
		dbglog(DBG_DEBUG, "G1ATA blkdev Failed to set partition 0 in pio mode\n");
		return -1;
	}
	dbglog(DBG_DEBUG, "*** PIO MODE ***\n");
	dbglog(DBG_DEBUG,
		   "~ G1 ATA Device Initialized\n~ Set drive to SLAVE and Partition 0 Selected\n");
	return 0;
}

extern int initFS(char *blkhdd)
{
	if (checkHDD())
	{
		dbglog(DBG_DEBUG, "* Failed to check HDD!\n");
		return -1;
	}
	if (fs_ext2_init())
	{
		dbglog(DBG_DEBUG, "* Failed ext2 FS Init!\n");
		return -1;
	}
	if (fs_ext2_mount(blkhdd, &bd_pio, MNT_MODE))
	{
		dbglog(DBG_DEBUG, "* Failed mounting in pio mode: /hd !\n");
		return -1;
	}
	mountState = 1;
	dbglog(DBG_DEBUG, "~ Initialized & Mounted the filesystem\n");
	return 0;
}

extern int removeFS(char *blkhdd)
{
	if (fs_ext2_unmount(blkhdd))
	{
		dbglog(DBG_DEBUG, "* Failed to unmount ext2fs!\n");
		return -1;
	}
	mountState = 0;
	dbglog(DBG_DEBUG, "~ Unmounted ext2fs\n");
	return 0;
}

extern int syncHDD(char *blkhdd)
{
	if (fs_ext2_sync(blkhdd))
	{
		dbglog(DBG_DEBUG, "* Failed to SYNC ext2fs!\n");
		return 1;
	}

	dbglog(DBG_DEBUG, "~ SYNC ext2fs\n");
	return 0;
}

extern int writeTextFile(char *blkhdd, char *str)
{
	FILE *fp;
	if (!(fp = fopen(blkhdd, "w")))
	{
		dbglog(DBG_DEBUG, "Could not create file: %s\n", strerror(errno));
		return 1;
	}

	if (fprintf(fp, str) < 0)
	{
		dbglog(DBG_DEBUG, "Failed to write to file: %s\n", strerror(errno));
		return 1;
	}
	else
	{
		dbglog(DBG_DEBUG, "\nWrote to file: %s\n", blkhdd);
	}

	fclose(fp);
	return 0;
}

extern int settingDBRead(char *blkhdd)
{
	FILE *fp;
	if (!(fp = fopen(blkhdd, "r")))
	{
		dbglog(DBG_DEBUG, "Could not open settings file: %s\n", strerror(errno));
		return 1;
	}

	fscanf(fp, "USER=%s MOTD=%s ID=%d", user, motd, &id);
	dbglog(DBG_DEBUG, "USERNAME = %s | MOTD = %s | ID = %d\n", user, motd, id);
	fclose(fp);
	return 0;
}

extern int settingDBWrite(char *blkhdd, char *userName, char *motday, int idcard)
{
	FILE *fp;
	if (!(fp = fopen(blkhdd, "w")))
	{
		dbglog(DBG_DEBUG, "Could not open settings file: %s\n", strerror(errno));
		return 1;
	}

	fprintf(fp, "USER=%s MOTD=%s ID=%d", userName, motday, idcard);
	dbglog(DBG_DEBUG, "\nSaved settings!\n");
	fclose(fp);
	return 0;
}

extern int writeRTF()
{
	FILE *fp;
	if (!(fp = fopen("/hd/test.txt", "w")))
	{
		dbglog(DBG_DEBUG, "Could not create file: %s\n", strerror(errno));
		return 1;
	}

	if (fprintf(fp, "HEADER TEST") < 0)
	{
		dbglog(DBG_DEBUG, "Failed to write to file: %s\n", strerror(errno));
		return 1;
	}
	else
	{
		dbglog(DBG_DEBUG, "\nWrote to file: /hd/test.txt\n");
	}

	fclose(fp);
	return 0;
}

extern int executeSub(char *rdsubelf)
{
	file_t f;
	void *subelf;
	f = fs_open(rdsubelf, O_RDONLY);
	assert(f);
	subelf = fs_mmap(f);
	assert(subelf);
	arch_exec(subelf, fs_total(f));
	return 0;
}

extern int makeDir(char *fileDir, int perm)
{
	struct stat st = {0};
	if (stat(fileDir, &st) == -1)
	{
    	mkdir(fileDir, perm);
		dbglog(DBG_DEBUG, "~ Made Directory %s with %d attributes\n", fileDir, perm);
	} else
	{
		dbglog(DBG_DEBUG, "* Failed to Make Directory %s\n", fileDir);
		return 1;
	}
	return 0;
}

extern void shutDown(char *blkhdd)
{
	syncHDD(blkhdd);
	removeFS(blkhdd);
	g1_ata_shutdown();
	dbglog(DBG_DEBUG, "\nShutdown Reached!\n\n");
	arch_exit();
}
