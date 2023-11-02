#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

int main(){
    // ftok to generate a unique key
    key_t key = ftok("shmfile", 65);

    // shmget returns an identifier in shmid
    // r-4 w-2 x-1 and 6 = 4+2 (r + w)
    int shmid = shmget(key, 70000, 0666|IPC_CREAT);

    // shmat to attach to shared memory
    char *str = (char *) shmat(shmid, (void *)0, 0);

    FILE *f1 = fopen("week05-ML2-input.txt", "r");
    
    int lines;
    fscanf(f1, "%d", &lines);
    lines+=1;
    //printf("%d",lines);
    
    char line[1000];
    strcpy(str, "");
    while(lines--){
    	fgets(line, 1000, f1);
    	//printf("Data written in memory: %s\n", line);
    	strcat(str, line);
    }
    
    fclose(f1);

    //printf("Data written in memory: %s\n", str);

    // detach from the memory 
    shmdt(str);

    return 0;
}
