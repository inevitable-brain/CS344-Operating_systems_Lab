#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define SHM_SIZE (1<<21)
union semun
{
    int val;            
    struct semid_ds *buf; 
    int *array;          
};

void sem_signal(int sem_id, int sem_num)
{
    struct sembuf sem_op;
    sem_op.sem_num = 0;
    sem_op.sem_op = 1;
    sem_op.sem_flg = 0;
    semop(sem_id, &sem_op, 1);
}

void sem_wait(int sem_id, int sem_num)
{
    struct sembuf sem_op;
    sem_op.sem_num = 0;
    sem_op.sem_op = -1;
    sem_op.sem_flg = 0;
    semop(sem_id, &sem_op, 1);
}

int shmid;
char *shared_memory;
int semid,readid;
FILE* f;

int main() {
    key_t shmkey = ftok("repository.c", 0);
    key_t semkey = ftok("writer.c", 'a');
    key_t readkey = ftok("reader.c", 'a');

    shmid = shmget(shmkey, SHM_SIZE, 0644);

    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    shared_memory = shmat(shmid, NULL, 0);

    semid = semget(semkey, 1, 0644);
    readid=semget(readkey, 1, 0644);

    f=fopen("tem.txt","w");
    fprintf(f,"%d",0);
    fclose(f);

    while(1){       
        sem_wait(readid, 1);
        int readcnt;
        f=fopen("tem.txt","r");
        fscanf(f,"%d",&readcnt);
        fclose(f);
        readcnt++;
        
        f=fopen("tem.txt","w");
        fprintf(f,"%d",readcnt);
        fclose(f);
         if(readcnt==1){
            sem_wait(semid,1);
        }
        sem_signal(readid,1);

        printf("%s",shared_memory);
        sleep(1);
        sem_wait(readid, 1);
        f=fopen("tem.txt","r");
        fscanf(f,"%d",&readcnt);
        
        fclose(f);
        readcnt--;
        f=fopen("tem.txt","w");
        fprintf(f,"%d",readcnt);
        
        fclose(f);
        if(readcnt==0){
            sem_signal(semid,1);
        }
        sem_signal(readid,1);

    }
    
    semctl(semid, 0, IPC_RMID); 
    semctl(readid, 0, IPC_RMID); 
    shmdt(shared_memory);
    
    return 0;
}
