#include <fs.h>

// Structure for a mounted filesystem
struct mount {
	int dev;
	int fs_id;
	struct superblock sb;
	struct icache icache;
};

struct file_system_type {
	int (*read_super)(int dev, struct superblock *sb);
	char * name;
};
