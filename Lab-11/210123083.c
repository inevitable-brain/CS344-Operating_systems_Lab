#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SIZE 400
int mat1[SIZE][SIZE];
int mat2[SIZE][SIZE];
int mat_res[SIZE][SIZE];

pthread_mutex_t mutex;

typedef struct {
    int start;
    int end;
} ThreadData;

void initialize(int m[SIZE][SIZE]) {
    srand(time(0));
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            m[i][j] = rand() % 10;
        }
    }
}

void *mat_mul(void *arg) {
    ThreadData *data = (ThreadData *)arg;
 
    for (int i = 0; i < SIZE; i++) {
    	for (int j = 0; j < SIZE; j++) {
	    int sum = 0;	    
            for (int k = data->start; k < data->end; k++) {
                sum += mat1[i][k] * mat2[k][j];
            }
            
            pthread_mutex_lock(&mutex);
            mat_res[i][j] += sum;
            pthread_mutex_unlock(&mutex);
        }
    }
 
    pthread_exit(NULL);
}

int main(){
	initialize(mat1);
	initialize(mat2);
	
	pthread_t threads[4];
	ThreadData threadData[4];
	
	pthread_mutex_init(&mutex, NULL);
	
	for(int i=0; i<SIZE; i++){
		for(int j=0; j<SIZE; j++){
			mat_res[i][j] = 0;
		}
	}
	
	for(int i=0; i<4; i++){
		threadData[i].start = i * 100;
        	threadData[i].end = (i + 1) * 100;
        	
        	pthread_create(&threads[i], NULL, (void *)&mat_mul, (void *)&threadData[i]);
	}
	
	for (int i = 0; i < 4; i++) {
        	pthread_join(threads[i], NULL);
    	}
    	
    	FILE *f1 = fopen("result.txt", "w");
    	for(int i=0; i<SIZE; i++){
    		for(int j=0; j<SIZE; j++){
    			fprintf(f1, "%d ", mat_res[i][j]);
    		}
		fprintf(f1, "\n");
    	}
    	
    	fclose(f1);
}
