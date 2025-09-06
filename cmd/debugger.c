#include "../include/errno.h"
#include "../include/stdio.h"
#include "../include/stat.h"
#include "../include/fcntl.h"

int
main()
{
  char* passwd=crypt("bingus", "po\0");
  printf("%s\n", passwd);

  for(int i = 0; i < 256; i++){
  	  putchar(i);
  }
  putchar('\n');

  return 0;
}
