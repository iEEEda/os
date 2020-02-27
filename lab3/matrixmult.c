#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

#define THREADS 2

int m1[2][3] = {{1, 2, 3},
		{4, 5, 6}};
int m2[3][2] = {{1, 2},
		{3, 4},
		{5, 6}};
int result[2][2] = {{0, 0},
			{0, 0}};

void empty_result();
void print_matrix();

void *multiply( void *i) {
	int *n = (int *) i;
	for (int j = 0; j < 2; j++) {
		for (int k = 0; k < 3; k++) {
			result[*n][j] = result[*n][j] + m1[*n][k] * m2[k][j];
		}
	}
	pthread_exit(NULL);
}

int main() {
	struct timeval tv;
	struct timeval tv2;
	gettimeofday(&tv, NULL);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 3; k++) {
				result[i][j] = result[i][j] + m1[i][k] * m2[k][j];
			}
		}
	}
	gettimeofday(&tv2, NULL);
	printf("Time taken: %ld\n", (tv2.tv_sec - tv.tv_sec) * 1000000 + tv2.tv_usec - tv.tv_usec);
	print_matrix();
	empty_result();
	pthread_t threads[THREADS];
	int status;
	int index[THREADS];
	gettimeofday(&tv, NULL);
	for ( int i = 0; i < THREADS; i++ ) {
		printf("Creating thread %d\n", i);
		index[i] = i;
		status = pthread_create( &threads[i], NULL, multiply, (void *) &index[i]);
		if (status != 0) {
			printf("pthread create returned error %d\n", status);
			return 1;
		}
	}
	for ( int i = 0; i < THREADS; i++ ) {
		pthread_join( threads[i], NULL);
		printf("Done %d\n", i);
	}
	gettimeofday(&tv2, NULL);
	printf("Time taken for threads: %ld\n", (tv2.tv_sec - tv.tv_sec) * 1000000 + tv2.tv_usec - tv.tv_usec);
	print_matrix();
}

void empty_result() {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			result[i][j] = 0;
		}
	}
}

void print_matrix() {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			printf("%d ", result[i][j]);
		}
		printf("\n");
	}
}
