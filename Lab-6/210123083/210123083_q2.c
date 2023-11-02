#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>
#include <math.h>
#include <sys/wait.h>

typedef struct my_msg{
	long int msg_type;
	int row;
	int column;
} my_msg;

int main(){
	FILE *f1 = fopen("ML2-input.txt", "r");
	int xr,xc;
	fscanf(f1, "%d %d", &xr, &xc);
	int Kx[xr][xc];
	
	for(int i=0; i<xr; i++){
		for(int j=0; j<xc; j++){
			fscanf(f1, "%d", &Kx[i][j]);
			//printf("%d ", Kx[i][j]);
		}
		//printf("\n");
	}
	
	int yr,yc;
	fscanf(f1, "%d %d", &yr, &yc);
	int Ky[yr][yc];
	
	for(int i=0; i<yr; i++){
		for(int j=0; j<yc; j++){
			fscanf(f1, "%d", &Ky[i][j]);
			//printf("%d ", Ky[i][j]);
		}
		//printf("\n");
	}
	
	int ir,ic;
	fscanf(f1, "%d %d", &ir, &ic);
	int I[ir][ic];
	
	for(int i=0; i<ir; i++){
		for(int j=0; j<ic; j++){
			fscanf(f1, "%d", &I[i][j]);
			//printf("%d ", I[i][j]);
		}
		//printf("\n");
	}
	
	fclose(f1);

	int msgid;
	key_t key;

	my_msg msg1;
	long int msg_to_rec = 0;

	// creating the same key
	key = ftok("msg_send_file.txt", 69);

	// connecting to the same msg queue and storing msgid
	msgid = msgget(key, 0666);
	
	FILE *f2 = fopen("output.txt", "w");

	for(;;)
	{
		for(int j=0; j<8; j++){
			if(msgrcv(msgid, (my_msg *)&msg1, sizeof(msg1), 0, 0) == -1){
				fclose(f2);
				exit(1);
			}
			pid_t pid = fork();
			if(pid == -1){
				printf("Error, Child Process could not be created!");
			}
			else if(pid == 0){
				float edge=0.0;
				int gx=0, gy=0;
				
				int r = msg1.row;
				int c = msg1.column;
				
				for(int i=-1; i<2; i++){
					for(int j = -1; j<2; j++){
						gx += I[r+i][c+j] * Kx[i+1][j+1];
						gy += I[r+i][c+j] * Ky[i+1][j+1];
					}
				}
				
				edge = sqrt(gx*gx + gy*gy);
				if(edge > 255) edge = 255;
				fprintf(f2, "(%d, %d):- %f\n", r, c, edge);
				exit(0);
			}
			else{
				continue;
			}
		}
		
		for(int j=0; j<8; j++){
			wait(NULL);
		}
	}
    	
    	return 0;
}
