/*
 * ved - visual editor
 */

#include <stdio.h>
#include <fcntl.h>
#include <stat.h>
#include <types.h>
#include <errno.h>

size_t buffsize = 1;
char *buff;
size_t modchar = 0;

int resize_buff(int size) {
	char *tmp = malloc(size);
	buffsize += size;
	strcpy(tmp, buff);
	buff = tmp;
	return 0;
}

// int insline(char *ins, int l, int r) {
// 	char *tmp1 = malloc(1);
// 	char *tmp2 = malloc(1);
// 	size_t s;
// 	resize_buff(strlen(ins));
//
// 	for(int i; i < strlen(buff) && l >= 0; i++) {
// 		if (buff[i] == '\n') {
// 			l--;
// 			s = i;
// 		}
// 	}
// 	if (s < modchar) {
// 		modchar = s;
// 	}
//
// 	tmp1 = malloc(s);
//
// }

int main (int argc, char *argv[]) {
	buff = malloc(buffsize);
	int fd = 0;
	struct stat st;

	if (argc > 1) {
		fd = open(argv[1], O_RDWR);
		stat(argv[1], &st);
		if (fd < 0) {
			perror(argv[1]);
			return 1;
		}
		printf("Opened file, size: %d\n", st.st_size);
		resize_buff(st.st_size);
		read(fd, buff, buffsize);
		modchar = st.st_size;
		lseek(fd, modchar, SEEK_SET);

		printf("\033[H\033[2J");
		printf("%s\n", buff);
		printf(">");
		resize_buff(16);
		strcat(buff, gets(stdin, 16));
		printf("%s\n", buff);
		memmove(buff, buff + modchar, buffsize - modchar + 1);
		printf("modified: %s\n", buff);
		write(fd, buff, strlen(buff));
	}

	return 0;
}
