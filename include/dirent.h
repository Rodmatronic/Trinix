#include <stdint.h>

// Directory is a file containing a sequence of dirent structures.
#define DIRSIZ 64

struct dirent {
    ino_t d_ino;
    off_t d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char d_name[DIRSIZ];
    char _pad[10];
};
