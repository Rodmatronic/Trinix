#include "../include/user.h"

int
main(argc, argv)
int argc;
char *argv[];
{
	register char *str;

	str = (argc > 1) ? argv[1]: "y";
	while (puts(str) != EOF)
		;
	exit(-1);
}
