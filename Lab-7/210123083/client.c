#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

struct sembuf sb = {0,-1,0};

void sem_wait(int semid){
    sb.sem_num = 0;
    sb.sem_op = 0;
    sb.sem_flg = 0;

    if(semop(semid, &sb, 1) == -1) {
        perror("semop");
    }         
}

void sem_signal(int semid){
    sb.sem_num = 0;
    sb.sem_op = 1;
    sb.sem_flg = 0;

    if(semop(semid, &sb, 1) == -1) {
        perror("semop");
    }
}

int main(){
    int semid;
    int shmid;
    int *requests;
    key_t key1, key2;

    // declaring the semaphore
    key2 = ftok("semaphore.txt", 67);
    semid = semget(key2, 1, 0);

    key1 = ftok("shared_mem.txt", 68);
    shmid = shmget(key1, 25, 0666);

    requests = shmat(shmid, (void *)0, 0);

    for(int i=0; i<100; i+=2){

        printf("Sending data %d and %d\n", i, i+1);

        sem_wait(semid);

        requests[0] = i;
        sem_signal(semid);

        requests[1] = i+1;
        sem_signal(semid);
    }

    if(shmdt(requests) == -1){
        perror("shmdt\n");
        exit(1);    
    }
}