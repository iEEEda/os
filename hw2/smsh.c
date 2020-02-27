/*
**	This program is a very simple shell that only handles 
**	single word commands (no arguments).
**	Type "quit" to quit.
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define CMDLEN 254
#define MAXARGS 100

int main()
{
	int pid;
	int status;
	int i, fd;
	int back = 0;
	char command[CMDLEN];

	printf( "Program begins.\n" );
	
	for (;;)
	{
		back = 0;
		printf( "smsh%%  " );
		fgets( command, CMDLEN, stdin );
		command[strlen(command)-1] = '\0';
		if ( strcmp(command, "quit") == 0 )
			break;
		if ( command[strlen(command)-1] == '&')
		{
			back = 1;
			command[strlen(command)-1] = '\0';
		}
		pid = fork();
		if ( pid < 0 )
		{
			printf( "Error in fork.\n" );
			exit(-1);
		}
		if ( pid != 0 )
		{
			printf( "PARENT. pid = %d, mypid = %d.\n", pid, getpid() );
			if (back == 0)
				waitpid( pid, &status, 0 );
		}
		else
		{
			printf( "CHILD. pid = %d, mypid = %d.\n", pid, getpid() );
			// if command is not recognized then the child process will become a second shell
			// execlp is not suitable for multiple args
			// execlp( command, command, NULL );
			char* ptr[MAXARGS];
			char* tok = strtok(command, " ");
			ptr[0] = tok;
			int i = 1;
			while (tok != NULL) {
				tok = strtok(NULL, " ");
				ptr[i] = tok;
				i++;
			}
			//printf("%d: %s, %d: %s, %d: %s, %d: %s\n", 0, ptr[0], 1, ptr[1], 2, ptr[2], 3, ptr[3]);
			//printf("i is: %d\n", i);
			if (i > 2) {
			if (strcmp(ptr[i - 3], ">") == 0 || strcmp(ptr[i - 3], ">>") == 0) {
				if (strcmp(ptr[i - 3], ">") == 0) {
					int fd1 = open(ptr[i - 2], O_RDWR | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
					dup2(fd1, 1);
				} else {
					int fd1 = open(ptr[i - 2], O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
					lseek(fd1, 0, SEEK_END);
					dup2(fd1, 1);
				}
				ptr[i - 3] = NULL;
			} //else if (*ptr[i - 3] == ">" || *ptr[i - 3] == ">>") {
			}
			execvp(command, ptr);
			// break will prevent what is written in the comments above
			printf("No such program: %s\n", command);
			break;
		}
	}
}
