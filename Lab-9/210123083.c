#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SHM_SIZE 500

typedef struct table{
	char name[50];
	int pid[10];
	int j;
} table;

union semun
{
    int val;
    struct semid_ds *buf;
    int *array;
};

void sem_signal(int sem_id, int sem_num)
{
    struct sembuf sem_op;
    sem_op.sem_num = sem_num;
    sem_op.sem_op = 1;
    sem_op.sem_flg = 0;
    semop(sem_id, &sem_op, 1);
}

void sem_wait(int sem_id, int sem_num)
{
    struct sembuf sem_op;
    sem_op.sem_num = sem_num;
    sem_op.sem_op = -1;
    sem_op.sem_flg = 0;
    semop(sem_id, &sem_op, 1);
}

int main(){
	int shmid, semid;
	union semun sem;
	table *shared_db;
	
	key_t key1 = ftok("210123083.c", 42);
	key_t key2 = ftok("210123083.c", 68);
	
	semid = semget(key1, 5, 0666 | IPC_CREAT);
	
	// Initialising all semaphores
	for(int i=0; i<5; i++){
		sem.val = 1;
		if(semctl(semid, i, SETVAL, sem) == -1){
	        	perror("Error initializing the semaphore.");
	        	exit(1);
		}
	}
	if((shmid = shmget(key2, SHM_SIZE, 0666 | IPC_CREAT)) == -1){
		perror("shmid");
	}
	
	shared_db = shmat(shmid, (void *)0, 0);
	
	table tables[5];
	for(int i=0; i<5; i++){
		sprintf(tables[i].name, "Table %d", i+1);
		tables[i].j = 0;
	}
	
	for(int i=0;i<5;i++){
		shared_db[i] = tables[i];
	}
	
	pid_t pid[5];
	for(int i=0; i<5; i++){
		if((pid[i] = fork()) == -1){
			perror("Error creating child");
		}
		else if(pid[i] == 0){//child i
			for(int j = 0; j<5; j++){
				int pos = (i+j)%5;
				
				int p_id = getpid();
				printf("Transaction %d holding the semaphore.\n", p_id);
				// acquiring appropriate semaphore
				//sleep(i);
				if(pos%2 == 0){
					sem_wait(semid, (pos+1)%5);
					sem_wait(semid, pos);
				}
				else {
					sem_wait(semid, pos);
					sem_wait(semid, (pos+1)%5);
				}		
				
				printf("Transaction %d: operating on %s - %s\n\n", p_id, shared_db[pos].name, shared_db[(pos+1)%5].name);
				shared_db[pos].pid[shared_db[pos].j] = p_id;
				shared_db[(pos+1)%5].pid[shared_db[(pos+1)%5].j] = p_id;
				
				shared_db[pos].j++;
				shared_db[(pos+1)%5].j++;
				
				sleep(1);
				
				if(pos%2 == 0){
					sem_signal(semid, pos);
					sem_signal(semid, (pos+1)%5);
				}
				else {
					sem_signal(semid, (pos+1)%5);
					sem_signal(semid, pos);
				}
			}	
			exit(0);
		}
		else{
			continue;
		}
	}
	
	for(int i=0; i<5; i++) wait(NULL);
	
	for(int i=0; i<5; i++){
		printf("%s:- ", shared_db[i].name);
		for(int j=0; j<10; j++){
			printf("%d ", shared_db[i].pid[j]);
		}
		printf("\n");
	}
	
	shmdt(shared_db);
	shmctl(shmid, IPC_RMID, NULL);
	
	semctl(semid, 0, IPC_RMID, sem);
	return 0;
}
