#include "../include/errno.h"
#include "../include/user.h"
#include "../include/stat.h"
#include "../include/fcntl.h"

int
main()
{
  unsigned long time_t = 0;
  char salt[3];

  struct stat st;
  int fd = open("/test", O_RDWR);
  fstat(fd, &st);
  printf("/test time: %d\n", st.lmtime);

  open("dd/ff/ff", O_CREATE|O_RDWR);

  char * np = "pou";
  salt[0] = np[0];
  salt[1] = np[1];
  salt[2] = '\0';
  printf("generated password from user %s: '%s'\n", np, crypt("dingus", salt));
  int rc = setuid(1);
  printf("setuid: %s\n", strerror(rc));
  execl("/bin/sh", "sh", 0);
  return 0;
}
