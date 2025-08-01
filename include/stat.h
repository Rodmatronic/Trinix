// File type and permission masks
#define S_IFMT  0170000  // Mask for file type
#define S_IFREG 0100000  // Regular file
#define S_IFDIR 0040000  // Directory
#define S_IFCHR 0020000  // Character device

// Permission bits
#define S_IRUSR 0400  // User read
#define S_IWUSR 0200  // User write
#define S_IXUSR 0100  // User execute
#define S_IRGRP 0040  // Group read
#define S_IWGRP 0020  // Group write
#define S_IXGRP 0010  // Group execute
#define S_IROTH 0004  // Other read
#define S_IWOTH 0002  // Other write
#define S_IXOTH 0001  // Other execute

struct stat {
  uint mode;     // File type and permissions
  uint ctime;    // creation time
  uint lmtime;	 // modification time
  int dev;       // File system's disk device
  uint ino;      // Inode number
  short nlink;   // Number of links to file
  uint size;     // Size of file in bytes
};
