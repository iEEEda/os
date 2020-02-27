#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdlib.h>

void empty_result();
void print_matrix();
long get_time();

int m1[1024][1024];
int m2[1024][1024];
int result[1024][1024];
int row;
int col;

void *multiply( void *i) {
	int *n = (int *) i;
	for (int j = 0; j < row; j++) {
		for (int k = 0; k < col; k++) {
			result[*n][j] = result[*n][j] + m1[*n][k] * m2[k][j];
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	if (argc != 5) {
		fprintf(stderr, "%d arguments required\n", 4);
		return 1;
	}			
	row = atoi(argv[1]);
	col = atoi(argv[2]);
	int val1 = atoi(argv[3]);
	int val2 = atoi(argv[4]);
	if (row > 1024 || col > 1024) {
		fprintf(stderr, "Maximum number of rows and columns is: %d\n", 1024);
		return 1;
	}
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			m1[i][j] = val1;
		}
	}
	for (int i = 0; i < col; i++) {
		for (int j = 0; j < row; j++) {
			m2[i][j] = val2;
		}
	}
	long start = get_time();
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < row; j++) {
			for (int k = 0; k < col; k++) {
				result[i][j] = result[i][j] + m1[i][k] * m2[k][j];
			}
		}
	}
	long end = get_time();
	fprintf(stderr, "Time taken sequentially: %ld\n", end - start);
	empty_result();
	pthread_t threads[row];
	int status;
	int index[row];
	start = get_time();
	for ( int i = 0; i < row; i++ ) {
		index[i] = i;
		status = pthread_create( &threads[i], NULL, multiply, (void *) &index[i]);
		if (status != 0) {
			return 1;
		}
	}
	for ( int i = 0; i < row; i++ ) {
		pthread_join( threads[i], NULL);
	}
	end = get_time();
	fprintf(stderr, "Time taken for pthreads: %ld\n", end - start);
	print_matrix();

	return 0;
}

void empty_result() {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < row; j++) {
			result[i][j] = 0;
		}
	}
}

void print_matrix() {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < row; j++) {
			printf(" %d ", result[i][j]);
		}
		printf("\n");
	}
}

long get_time() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	long time = tv.tv_usec;
	time = time + tv.tv_sec * 1000000;
	return time;
}
