#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS	10

void *print_hello( void *tid ) 
{
	int *id = (int *) tid;

//	sleep(1);
	printf( "Hello from thread %d.\n", *id );
//	pthread_exit( NULL );
}

int main( int argc, char **argv )
{
	pthread_t threads[THREADS];
	int status, i;

	for ( i = 0; i < THREADS; i++ )
	{
		printf( "Creating thread %d.\n", i );
		status = pthread_create( &threads[i], NULL, 
				print_hello, (void *) &i );
		if ( status != 0 )
			exit( -1 );
	}
//	pthread_exit( 0 );
}
