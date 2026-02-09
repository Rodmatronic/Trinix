// ../include/tty.h
#ifndef _TTY_H_
#define _TTY_H_

#include <spinlock.h>

struct cons {
  struct spinlock lock;
  int locking;
};

#endif
