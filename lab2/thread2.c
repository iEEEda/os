#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define THREADS	10

void *print_hello( void *tid )
{
	int id = *((int*) tid);

	printf( "Hello from thread %d.\n", id );
	pthread_exit( NULL );
}

int main( int argc, char **argv )
{
	pthread_t threads[THREADS];
	int status, i;
	int index[THREADS];

	for ( i = 0; i < THREADS; i++ )
	{
		printf( "Creating thread %d.\n", i );
		index[i] = i;
		status = pthread_create( &threads[i], NULL, 
				print_hello, (void *) &index[i] );
		if ( status != 0 )
		{
			printf( "pthread_create returned error %d.\n", 
				status );
			exit( -1 );
		}
	}
	printf( "Done creating threads.\n" );
	for ( i = 0; i < THREADS; i++ )
	{
		pthread_join( threads[i], NULL );
		printf( "Done %d.\n", i );
	}
	printf( "Done creating threads.\n" );
	pthread_exit( 0 );
}
