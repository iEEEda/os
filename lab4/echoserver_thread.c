#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <pthread.h>

#define	QLEN			5
#define	BUFSIZE			4096

int n;
int passivesock( char *service, char *protocol, int qlen, int *rport );

//struct user {
//	int sock;
//	char buf[4096];
//};

void *listen_user(void *i) {
	int *sock = (int *) i;
	char buf[BUFSIZE];
	int cc;
	for (;;) {
		if ((cc = read (*sock, buf, BUFSIZE)) <= 0 ) {
			printf("The client has gone.\n");
			close(*sock);
			break;
		} else {
			buf[cc] = '\0';
			printf("The client says: %s\n", buf);
			if (write( *sock, buf, cc) < 0) {
				close( *sock);
				break;
			}
		}
	}
	n--;
	pthread_exit(NULL);
}

int main( int argc, char *argv[] )
{
	char			*service;
	struct sockaddr_in	fsin;
	socklen_t		alen;
	int			msock;
	int			ssock[QLEN];
	int			rport = 0;
	pthread_t 		threads[QLEN];
	
	switch (argc) 
	{
		case	1:
			// No args? let the OS choose a port and tell the user
			rport = 1;
			break;
		case	2:
			// User provides a port? then use it
			service = argv[1];
			break;
		default:
			fprintf( stderr, "usage: server [port]\n" );
			exit(-1);
	}

	msock = passivesock( service, "tcp", QLEN, &rport );
	if (rport)
	{
		//	Tell the user the selected port
		printf( "server: port %d\n", rport );	
		fflush( stdout );
	}
	
	n = 0;
	for (;;)
	{
		if ( n < 4)  {
			alen = sizeof(fsin);
			ssock[n] = accept( msock, (struct sockaddr *)&fsin, &alen );
			if (ssock[n] < 0)
			{
				fprintf( stderr, "accept: %s\n", strerror(errno) );
				exit(-1);
			}
			printf( "A client has arrived for echoes.\n" );
			fflush( stdout );
	
			int status = pthread_create( &threads[n], NULL, listen_user, &ssock[n]);
			n++;
			if (status != 0) {
				exit(-1);
			}
		} else {
			printf("The socket is full\n");
			fflush( stdout);
		}
	}
}


