#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>

int insert_at(int fd, off_t offset, char *buf, int n);

int main(int argc, char *argv[]) {
	int fd, fd2, off, n, start;
	if (argc != 5) {
		printf("%s\n","Not correct number of arguments");
		exit(-1);
	} else {
		fd = open(argv[1], O_RDONLY);
		if (fd != -1) {
			printf("%d\n", fd);
			//open another file to write into, it may not exist
			fd2 = open(argv[2], O_RDWR | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
			printf("%d\n", fd2);
			off = atoi(argv[3]);
			n = atoi(argv[4]);
			//allocate memory for characters
			char *arr = (char *) calloc(n, sizeof(char));
			// read chars from the source file
			start = lseek(fd, off, SEEK_SET);
			for(int i = off; i < off + n; i++){
				read(fd, arr, 8 * n);
			}
			int end = lseek(fd, 0, SEEK_END);
			//if(off + n > end) {
			//	exit(-1);
			//}
			//write them to the new file
			int off2 = insert_at(fd2, off, arr, n);
			free(arr);
			if (off2 < 0) {
				printf("%s", "Some problems with the file");
				exit(-1);
			} else {
				printf("%s %d\n", "The offset past the replaced chars:", off);
				close(fd);
				close(fd2);
			}
		} else {
			printf("%s\n", "File doesn't exist or cannot be opened");
			exit(-1);
		}
	}
}

int insert_at(int fd, off_t offset, char *buf, int n) {
	int size = lseek(fd, 0, SEEK_END);
	int off = lseek(fd, offset, SEEK_SET);
	// save everything after the offset
	char *arr = (char *) calloc(size, sizeof(char));
	read(fd, arr, (size - offset));
	lseek(fd, offset, SEEK_SET);
	// write needed text
	write(fd, buf, n);
	// put saved text
	write(fd, arr, size - offset);
	free(arr);
	return size + n;
}
