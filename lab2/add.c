#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS	1000

int sum;

void *add( void *ign )
{
	int *i = (int *) ign;
	sum++;
	printf( "add %d: sum is %d.\n", *i, sum );
	pthread_exit( NULL );
}

void *subtract( void *ign )
{
	int *i = (int *) ign;
	sum--;
	printf( "sub %d: sum is %d.\n", *i, sum );
	pthread_exit( NULL );
}

int main( int argc, char **argv )
{
	int ids[THREADS*2];
	pthread_t threads[THREADS*2];
	int status, i, j;

	sum = 0;
	for ( j = 0, i = 0; i < THREADS; i++ )
	{
		ids[j] = i;
		status = pthread_create( &threads[j++], NULL, add, (void *) &ids[j] );
		if ( status != 0 )
		{
			printf( "pthread_create error %d.\n", status );
			exit( -1 );
		}
		ids[j] = i;
		status = pthread_create( &threads[j++], NULL, subtract, (void *) (ids+j) );
		if ( status != 0 )
		{
			printf( "pthread_create returned error %d.\n", 
				status );
			exit( -1 );
		}
	}
	for ( j = 0; j < THREADS*2; j++ )
		pthread_join( threads[j], NULL );
	printf( "Finally, the sum is %d.\n", sum );
	pthread_exit( 0 );
}
