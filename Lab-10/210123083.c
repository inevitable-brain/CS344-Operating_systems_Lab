#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct
{
	int row;
	int column;
} parameters;

int sudoku[9][9];

void *check_all_cols(){
	int temp = 1;
	int *all_cols = (int *)malloc(sizeof(int));
	*all_cols = temp;
	for(int j = 0; j < 9; j++){
		int arr[9] = {0,0,0,0,0,0,0,0,0};
		for(int i = 0; i < 9; i++){
			arr[sudoku[i][j]-1] = 1;
		}
		for(int k=0; k<9; k++){
			if(arr[k] == 0) *all_cols = 0;
		}
	}
	return (void *)all_cols;
}

void *check_all_rows(){
	int temp = 1;
	int *all_rows = (int *)malloc(sizeof(int));
	*all_rows = temp;
	for(int i = 0; i < 9; i++){
		int arr[9] = {0,0,0,0,0,0,0,0,0};
		for(int j = 0; j < 9; j++){
			arr[sudoku[i][j]-1] = 1;
		}
		for(int k=0; k<9; k++){
			if(arr[k] == 0) *all_rows = 0;
		}
	}
	return (void *)all_rows;
}

void *check_grid(void *arg){
	parameters p = *(parameters *)arg;
	int r = p.row;
	int c = p.column;
	int temp = 1;
	int *grid = (int *)malloc(sizeof(int));
	*grid = temp;
	int arr[9] = {0,0,0,0,0,0,0,0,0};
	for(int i = r; i < r+3; i++){
		for(int j = c; j < c+3; j++){
			arr[sudoku[i][j]-1] = 1;
		}
	}
	for(int k=0; k<9; k++){
		if(arr[k] == 0) *grid = 0;
	}
	return (void *)grid;
}

void *check_col(void *arg){
	int j = *(int *)arg;
	int temp = 1;
	int *col = (int *)malloc(sizeof(int));
	*col = temp;
	
	int arr[9] = {0,0,0,0,0,0,0,0,0};
	for(int i = 0; i < 9; i++){
		arr[sudoku[i][j]-1] = 1;
	}
	for(int k=0; k<9; k++){
		if(arr[k] == 0) *col = 0;
	}
	return (void *)col;
}

void *check_row(void *arg){
	int i = *(int *)arg;
	int temp = 1;
	int *row = (int *)malloc(sizeof(int));
	*row = temp;
	
	int arr[9] = {0,0,0,0,0,0,0,0,0};
	for(int j = 0; j < 9; j++){
		arr[sudoku[i][j]-1] = 1;
	}
	for(int k=0; k<9; k++){
		if(arr[k] == 0) *row = 0;
	}
	return (void *)row;
}

void *check_all_grids(){
	int temp = 1;
	int *all_grids = (int *)malloc(sizeof(int));
	*all_grids = temp;
	for(int p = 0; p<9; p++){
		int r = p/3;
		r *= 3;
		int c = (p*3)%9;
		int arr[9] = {0,0,0,0,0,0,0,0,0};
		for(int i = r; i < r+3; i++){
			for(int j = c; j < c+3; j++){
				arr[sudoku[i][j]-1] = 1;
			}
		}
		for(int k=0; k<9; k++){
			if(arr[k] == 0) *all_grids = 0;
		}
	}
	return (void *)all_grids;
}

int main(){
	FILE *f1 = fopen("week10-ML2-input1.txt", "r");
	for(int i=0; i<9; i++){
		for(int j=0; j<9; j++){
			fscanf(f1, "%d", &sudoku[i][j]);
			printf("%d ", sudoku[i][j]);
		}
		printf("\n");
	}
	fclose(f1);
	
	// method 1
	printf("######### Medthod - 1 #########\n");
	int res = 1;
	
	int **thread_check = malloc(11*sizeof(int *));
	
	//int *all_cols = 0;
	//int *all_rows = 0;
	//int **all_grids = malloc(9*sizeof(int *));
	
	for(int i=0; i<11; i++){
		thread_check[i] = malloc(sizeof(int));
		*thread_check[i] = 0;
	};
	//printf("ok\n");
	
	pthread_t t1, t2, t3[9];
	
	pthread_create(&t1, NULL, (void *)&check_all_cols, NULL);
	pthread_join(t1, (void **)&thread_check[0]);
	//res *= (*thread_check[0]);
	printf("All columns = %d\n", *thread_check[0]);
	
	pthread_create(&t2, NULL, (void *)&check_all_rows, NULL);
	pthread_join(t2, (void **)&thread_check[1]);
	//res *= (*thread_check[1];
	printf("All rows = %d\n\n", *thread_check[1]);
	
	for(int i=2; i<11; i++){
		parameters p1;
		p1.row = (i-2)/3;
		p1.row *= 3;
		p1.column = ((i-2)*3)%9;
		pthread_create(&t3[i-2], NULL, (void *)&check_grid, &p1);
		pthread_join(t3[i-2], (void **)&thread_check[i]);
		
		printf("grid %d = %d\n",i-1, *thread_check[i]);
		//res *= (*all_grids[i]);
	}
	
	//printf("%d\n", *all_rows);
	//printf("%d\n", *all_cols);
	
	for(int i=0; i<11; i++){
		if(*thread_check[i] == 0) res = 0;
	}
	
	if(res == 1) printf("Sudoku is correct!\n");
	else printf("Sudoku is incorrect!\n");
	
	// method 2
	printf("######### Medthod - 2 #########\n");
	res = 1;
	
	int **thread_check2 = malloc(19*sizeof(int *));

	for(int i=0; i<19; i++){
		thread_check2[i] = malloc(sizeof(int));
		*thread_check2[i] = 0;
	};
	//printf("ok\n");
	
	pthread_t t4[9], t5[9], t6;
	for(int i=0; i<9; i++){
		pthread_create(&t4[i], NULL, (void *)&check_col, &i);
		pthread_join(t4[i], (void **)&thread_check2[i]);
		printf("column %d = %d\n", i+1, *thread_check2[i]);
	}
	
	for(int i=0; i<9; i++){
		pthread_create(&t5[i], NULL, (void *)&check_row, &i);
		pthread_join(t5[i], (void **)&thread_check2[i+9]);
		printf("row %d = %d\n", i+1, *thread_check2[i+9]);
	}
	
	pthread_create(&t6, NULL, (void *)&check_all_grids, NULL);
	pthread_join(t6, (void **)&thread_check2[18]);
	
	printf("All grids = %d\n\n", *thread_check2[18]);
	
	for(int i=0; i<19; i++){
		if(*thread_check2[i] == 0) res = 0;
	}
	
	if(res == 1) printf("Sudoku is correct!\n");
	else printf("Sudoku is incorrect!\n");
	
	// method 3
	printf("######### Medthod - 3 #########\n");
	res = 1;
	
	int **thread_check3 = malloc(27*sizeof(int *));

	for(int i=0; i<27; i++){
		thread_check3[i] = malloc(sizeof(int));
		*thread_check3[i] = 0;
	};
	//printf("ok\n");
	
	pthread_t t7[9], t8[9], t9[9];
	for(int i=0; i<9; i++){
		pthread_create(&t7[i], NULL, (void *)&check_col, &i);
		pthread_join(t7[i], (void **)&thread_check3[i]);
		printf("column %d = %d\n", i+1, *thread_check3[i]);
	}
	
	for(int i=0; i<9; i++){
		pthread_create(&t8[i], NULL, (void *)&check_row, &i);
		pthread_join(t8[i], (void **)&thread_check3[i+9]);
		printf("row %d = %d\n", i+1, *thread_check3[i+9]);
	}
	
	for(int i=18; i<27; i++){
		parameters p1;
		p1.row = (i-18)/3;
		p1.row *= 3;
		p1.column = ((i-18)*3)%9;
		pthread_create(&t9[i-18], NULL, (void *)&check_grid, &p1);
		pthread_join(t9[i-18], (void **)&thread_check3[i]);
		
		printf("grid %d = %d\n",i-17, *thread_check3[i]);
		//res *= (*all_grids[i]);
	}
	
	for(int i=0; i<27; i++){
		if(*thread_check3[i] == 0) res = 0;
	}
	
	if(res == 1) printf("Sudoku is correct!\n");
	else printf("Sudoku is incorrect!\n");
	
	return 0;
}
