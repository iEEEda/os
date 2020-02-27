#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc, char *argv[]) {
	int fd;
	if (argc != 4) {
		printf("%s\n","Not correct number of arguments");
		return 1;
	} else {
		fd = open(argv[1], O_RDWR);
		if (fd != -1) {
			printf("%d\n", fd);
			return 0;
		} else {
			printf("%s\n", "File doesn't exist or cannot be opened");
			return 1;
		}
	}
}
