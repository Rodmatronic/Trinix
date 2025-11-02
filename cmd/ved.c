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

int writebufftofd(int *fd){
	char *writebuff = malloc(strlen(buff));
	strcpy(writebuff, buff);
	memmove(writebuff, writebuff + modchar, buffsize - modchar + 1);
	lseek(fd, modchar, SEEK_SET);
	write(fd, writebuff, strlen(writebuff));
	return 0;
}

int cmd(int *fd, struct stat st) {
	char cmdline[512];

	read(fd, buff, buffsize);
	modchar = st.st_size;

	printf("\033[H\033[2J");
	printf("%s\n", buff);

	printf("DEBUG:\n");
	printf("File size: %d\n", st.st_size);
	printf("modchar: %d\n", modchar);
	printf("buffer size: %d\n", buffsize);
	printf("buffer string length: %d\n", strlen(buff));

	printf("cmd>");
	strcpy(cmdline, gets(stdin, sizeof(cmdline)));

	switch (cmdline[0]) {
		case 'q':
			exit(0);
			break;
		case 'w':
			writebufftofd(fd);
			break;
		default:
			resize_buff(strlen(cmdline));
			strcat(buff, cmdline);

	}

	return 0;
}

int main (int argc, char *argv[]) {
	buff = malloc(buffsize);
	int fd;
	struct stat st;

	if (argc > 1) {
		fd = open(argv[1], O_RDWR | O_CREAT);
		if (fd < 0) {
			perror(argv[1]);
			return 1;
		}
		stat(argv[1], &st);
		resize_buff(st.st_size-1);
		while(1) {
			stat(argv[1], &st);
			cmd(fd, st);
		}
	}

	return 0;
}
