/*
 * ps - list user processes
 */

#include <stdio.h>
#include <types.h>
#include <pwd.h>
#include <errno.h>
#include <limits.h>

int jflg;
int aflg;
int uflg;
long uargl;
char uargstr[128];
char usrcmp[128];
int errflg;

usage()		/* print usage message and quit */
{
	static char usage[] = "ps [ -aj ] [ -u uid ] [ -U username ]";

	fprintf(stderr, "usage: %s\n", usage);
	exit(1);
}

int printprocs() {
	struct uproc proc;
	int pamount = devctl(3, 0, 0);
	int isusr = 1;
	register struct passwd *pw;
	printf("    PID");
	if (jflg) printf("   PGID");
	printf(" USER     ");
	printf("CMD");
	printf("\n");
	for (int i = 0; i < pamount; i++) {

		if (!getproc(i, &proc)) {
			pw = getpwuid(proc.p_uid);
			if (!aflg) {
				if (proc.p_uid == getuid()) isusr = 1;
				else isusr = 0;
			}
			if (uflg == 1){
				if (uargl == proc.p_uid) isusr = 1;
				else isusr = 0;
			} else if (uflg == 2 && pw) {
				strcpy(usrcmp, pw->pw_name);
				if (!strcmp(uargstr, usrcmp)) isusr = 1;
				else isusr = 0;
			}
			if (isusr) {
				printf("%7d ", proc.p_pid);
				if (jflg) printf("%6d ", proc.p_gid);
				if (pw) printf("%-8s ", pw->pw_name);
				else printf("%-8s ", proc.p_uid);
				printf("%s\n", proc.name);
			}

		}
	}
	return 0;
}

int
main (int argc, char *argv[]) {
	char c;
	int pgerrflg = 0;

	setprogname(argv[0]);

	while ((c = getopt(argc, argv, "jau:U:")) != EOF) {
		switch (c) {
			case 'j':
				jflg++;
				break;
			case 'a':
				aflg++;
				break;
			case 'u':
				uflg++;
				uargl = strtoul(optarg, NULL, 10);
				if (uargl > UID_MAX || errno == ERANGE) {
					fprintf(stderr, "%s: UID: %s\n", getprogname(), strerror(EINVAL));
					errflg++;
				}
				break;
			case 'U':
				uflg = 2;
				strcpy(uargstr, optarg);
				if (strlen(optarg) > 128) {
					fprintf(stderr, "%s: username: %s\n", getprogname(), strerror(EINVAL));
					errflg++;
				}
				break;
			default:
				errflg++;
				break;
		}
	}
	if (errflg || optind < argc || pgerrflg)
		usage();

	printprocs();
}
