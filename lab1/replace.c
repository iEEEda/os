#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

int replace_at(int fd, off_t offset, char *s, int n);

int main(int argc, char *argv[]) {
	int fd;
	if (argc != 4) {
		printf("%s\n","Not correct number of arguments");
		return 1;
	} else {
		fd = open(argv[1], O_RDWR);
		if (fd != -1) {
			printf("%d\n", fd);
			int off = replace_at(fd, atoi(argv[2]), argv[3], strlen(argv[3]));
			if (off < 0) {
				printf("%s", "Some problems with the file");
				exit(-1);
			} else {
				printf("%s %d", "The offset past the replaced chars:", off);
				close(fd);
			}
		} else {
			printf("%s\n", "File doesn't exist or cannot be opened");
			exit(-1);
		}
	}
}

int replace_at(int fd, off_t offset, char *s, int n) {
	int end = lseek(fd, 0, SEEK_END);
	if(offset + n > end) {
		exit(-1);
	} else {
		int off = lseek(fd, offset, SEEK_SET);
		int new_off = off + write(fd, s, n);
		return new_off;
	}
}
