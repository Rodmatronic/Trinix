#include <types.h>
#include <stat.h>
#include <stdio.h>
#include <fs.h>
#include <fcntl.h>
#include <tty.h>

#define BUF_SIZE 1024
#define HOSTNAME_MAX 256

char* getentry(char * entry) {
	int fd = open("/etc/gettytab", O_RDONLY);
	if (fd < 0) return NULL;

	static char line[BUF_SIZE];
	char buf[BUF_SIZE];
	ssize_t n;
	int i = 0;
	int found_default = 0;

	while ((n = read(fd, buf, BUF_SIZE)) > 0) {
		for (ssize_t j = 0; j < n; j++) {
			char c = buf[j];
			if (c == '\n' || c == '\0') {
				line[i] = '\0';
				if (found_default) {
					close(fd);
					return line;
				}
				if (strcmp(line, entry) == 0) {
					found_default = 1;
				}
				i = 0;
			} else {
				if (i < BUF_SIZE - 1) line[i++] = c;
			}
		}
	}

	close(fd);
	return NULL;
}

// process the entry from gettytab
void processline(char* src, char* dst, size_t size) {
	if (!src || !dst) return;
	char* p = src;

	size_t j = 0;
	char hostname[HOSTNAME_MAX+1];
	gethostname(hostname, sizeof(hostname));
	hostname[HOSTNAME_MAX] = '\0';

	char tty[PATH_MAX];
	if (!strcpy(tty, basename(ttyname(STDIN_FILENO))))
		strcpy(tty, "???");
	while (*p && j < size - 1) {
		if (*p == '\\' && *(p+1) == 'r') {
			dst[j++] = '\r';
			p += 2;
		} else if (*p == '\\' && *(p+1) == 'n') {
			dst[j++] = '\n';
			p += 2;
		} else if (*p == '%' && *(p+1) == 'h') {
			size_t k = 0;
			while (hostname[k] && j < size - 1) dst[j++] = hostname[k++];
			p += 2;
		} else if (*p == '%' && *(p+1) == 't') {
			size_t k = 0;
			while (tty[k] && j < size - 1) dst[j++] = tty[k++];
			p += 2;
		} else if (*p == ':' && *(p+1) == '\0') {
			break; // remove final colon
		} else {
			dst[j++] = *p++;
		}
	}

	dst[j] = '\0';
}

int
main(int argc, char **argv)
{
	int opt;
	char banner[BUF_SIZE];
	while ((opt = getopt(argc, argv, "")) != -1) {
		switch(opt) {
		default:
			fprintf(stderr, "usage: getty\n");
			exit(1);
		}
	}
	if (argc < 2){
		fprintf(stderr, "getty: not enough args\n");
		exit(1);
	}
	char *line = getentry("default:\\");
	if (line) {
		processline(line, banner, BUF_SIZE);
		printf("%s", banner);
	}
	for (;;) {
		execl("/sbin/login", "login", (char *)NULL);
	}
	return 1;
}
