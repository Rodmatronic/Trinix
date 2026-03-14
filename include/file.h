struct file {
	enum { FD_NONE, FD_PIPE, FD_INODE } type;
	int ref; // reference count
	char readable;
	char writable;
	struct pipe *pipe;
	struct inode *ip;
	unsigned int off;
	unsigned int flags;
};

/*
 * TODO: Replace MAJOR/MINOR with one feild, should probably re-use 'dev'.
 */

// in-memory copy of an inode
struct inode {
	unsigned int dev;	// Device number
	unsigned int inum;	// Inode number
	int ref;		// Reference count
	struct sleeplock lock;	// protects everything below here
	int valid;		// inode has been read from disk?
	short mode;		// Inode's UNIX mode
	unsigned int lmtime;
	unsigned int ctime;
	short major;		// Device's MAJOR number
	short minor;		// Device's MINOR number
	short nlink;		// Number of links to inode
	unsigned int size;	// Size of the inode in bytes
	unsigned int addrs[NDIRECT+1];
	unsigned int uid;	// UID permissions
	unsigned int gid;	// GID permissions
};

struct icache {
	struct spinlock lock;
	struct inode inode[NINODE];
};

extern struct icache icache;

/*
 * Both tables here map a major/minor device number
 * to a function, but device_inode_table is for file
 * reading, while device_block_table is for direct
 * VFS access.
 */
struct device_inode_table {
	int (*read)(int minor, struct inode*, char*, int, uint32_t);
	int (*write)(int minor, struct inode*, char*, int, uint32_t);
};

struct device_block_table {
	int (*read)(int minor, struct buf *b);
	int (*write)(int minor, struct buf *b);
};

extern struct device_inode_table device_inode_table[];
extern struct device_block_table device_block_table[];
