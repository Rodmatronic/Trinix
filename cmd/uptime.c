#include "../include/types.h"
#include "../include/stat.h"
#include "../include/stdio.h"
#include "../include/stdbool.h"
#include "../include/errno.h"

int main(void) {
    int seconds = uptime() / 2;
    int mins = seconds / 60;
    int hrs = mins / 60;
    mins %= 60;
    seconds %= 60;

    printf("up %d:%02d:%02d\n", hrs, mins, seconds);
    return 0;
}
