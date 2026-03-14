#include <types.h>
#include <defs.h>
#include <param.h>
#include <stat.h>
#include <mmu.h>
#include <proc.h>
#include <spinlock.h>
#include <sleeplock.h>
#include <fs.h>
#include <buf.h>
#include <file.h>
#include <errno.h>
#include <dirent.h>
#include <major.h>
#include <vfs.h>

int ROOTDEV = MKDEV(IDE0_MAJOR, 1);
struct icache icache;

struct mount mounts[NMOUNT];

int xv6fs_read_super(int dev, struct superblock *sb);

struct file_system_type filesystems[] = {
        {xv6fs_read_super, "xv6fs"},
        {0, 0}
};

struct device_block_table device_block_table[NDEV] = {
	[IDE0_MAJOR]	= {ide_block_read, ide_block_write},
//	[FLOPPY_MAJOR]	= {floppy_block_read, floppy_block_write},
};

struct superblock * get_superblock(int dev){
	for (int i = 0; i < NMOUNT; i++){
		if(mounts[i].dev == dev)
			return &mounts[i].sb;
	}
	panic("getsb: unknown filesystem device");
}

int xv6fs_read_super(int dev, struct superblock *sb){
	struct buf *bp;

	bp = buffer_read(dev, 1);
	memmove(sb, bp->data, sizeof(*sb));
	buffer_release(bp);

	if (sb->size == 0 || sb->nblocks == 0)
		return 1;

	debug("size       : %d\n", sb->size);
	debug("nblocks    : %d\n", sb->nblocks);
	debug("ninodes    : %d\n", sb->ninodes);
	debug("nlog       : %d\n", sb->nlog);
	debug("logstart   : %d\n", sb->logstart);
	debug("inodestart : %d\n", sb->inodestart);
	debug("bmap start : %d\n", sb->bmapstart);
	return 0;
}

int mount(int dev){
	for (int i = 0; i < NMOUNT; i++){
		if (mounts[i].dev == 0){
			mounts[i].dev = dev;
			return i;
		}
	}
	return -1;
}

/*
 * Try and find a usable filesystem on ROOTDEV
 */
void vfs_mount_root(void){
	int i = 0;

	mount(ROOTDEV);	// 0 is always root
	initlock(&icache.lock, "icache");

	for (i = 0; i < NINODE; i++)
		initsleeplock(&icache.inode[i].lock, "inode");
	if ((MAJOR(ROOTDEV) == FLOPPY_MAJOR)){
		printk("Insert root floppy and press ENTER\n");
		while (kgetchar() != '\n');
	}
	for (int f = 0; filesystems[f].name != 0; f++){
		if (filesystems[f].read_super(ROOTDEV, &mounts[0].sb) == 0){
			printk("Mounted root device (%d,%d) type %s\n", MAJOR(ROOTDEV), MINOR(ROOTDEV), filesystems[f].name);
			return;
		}
	}
	panic("Could not mount root (%d,%d)", MAJOR(ROOTDEV), MINOR(ROOTDEV));
}
