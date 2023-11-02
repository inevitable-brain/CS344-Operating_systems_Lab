#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>

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
    sb.sem_op = -1;
    sb.sem_flg = 0;

    if(semop(semid, &sb, 1) == -1) {
        perror("semop");
    }
}

int main(){
    int semid;
    int shmid;
    union semun arg;
    int req = 0;
    int *requests;
    key_t key1, key2;

    key1 = ftok("shared_mem.txt", 68);
    shmid = shmget(key1, 25, 0666 | IPC_CREAT);

    // creating the semaphore
    key2 = ftok("semaphore.txt", 67);
    semid = semget(key2, 1, 0666 | IPC_CREAT);

    // initializing the semaphore
    arg.val = 1;
    if(semctl(semid, 0, SETVAL, arg) == -1){
        perror("Error initializing the semaphore.");
        exit(1);
    }

    pid_t pid1, pid2;
    if((pid1 = fork()) < 0){
        perror("Error Creating Child");
    }
    else if(pid1 == 0){
        requests = shmat(shmid, (void *)0, 0);
        
        while(1){
            sem_signal(semid);

            int n1 = requests[0];
            printf("Http request: %d, received by child 1\n", n1);

            if(n1==98){
                break;
            }
        }
        if(shmdt(requests)==-1){
            perror("shmdt\n");
            exit(1);
        }
        exit(0);
    }
    else if(pid1 > 0){
        if((pid2 = fork()) < 0){
            perror("Error Creating Child");
        }
        else if(pid2 == 0){
            requests = shmat(shmid, (void *)0, 0);
        
            while(1){
                sem_signal(semid);
                int n2 = requests[1];

                printf("Http request: %d, received by child 2\n", n2);

                if(n2==99){
                    break;
                }
            }
            if(shmdt(requests)==-1){
                perror("shmdt\n");
                exit(1);
            }
            exit(0);
        }
        else if(pid2 > 0){
            wait(NULL);
            wait(NULL);
        }
    }
    // removing the semaphore
    if(semctl(semid, 0, IPC_RMID, arg) == -1){
        perror("Semaphore could'nt be removed");
        exit(1);
    }
    if(shmctl(shmid, IPC_RMID, NULL) == -1){
        perror("shmctl\n");
        exit(1);
    }
    return 0;
}