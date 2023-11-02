#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#define WRITER_NO 2
#define SHM_SIZE (1<<21)
union semun
{
    int val;              /* used for SETVAL only */
    struct semid_ds *buf; /* for IPC_STAT and IPC_SET */
    int *array;           /* used for GETALL and SETALL */
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
int semid;

int main() {
    key_t key = ftok("repository.c", 0);
    key_t semkey = ftok("writer.c", 'a');
    pid_t pid[WRITER_NO];
    shmid = shmget(key, SHM_SIZE, 0644);

    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    shared_memory = shmat(shmid, NULL, 0);

    semid = semget(semkey, 1, 0644);
    for(int i=0;i<WRITER_NO;i++){
        pid[i]=fork();
        if(pid[i]==0){
            while(1){
		
		sem_wait(semid, 1);

		int sz=strlen(shared_memory);
		shared_memory[sz-2]='\0';
		strcat(shared_memory,"\tprintf(\"Hello written by pid: ");
		int x=getpid();
		char no[10];
		int i=0;
		while(x){
		    int y=x%10;
		    no[i++]=y+'0';
		    x/=10;
		}
		no[i]='\0';
		i--;
		int j=0;
		while(i<j){
		    char c=no[j];
		    no[j]=no[i];
		    no[i]=c;
		}
		strcat(shared_memory,no);
		strcat(shared_memory,"\");\n}\n");
		sleep(1);
		printf("%s",shared_memory);
		sem_signal(semid, 1);
	    }
            exit(0);
        }
    }
    for(int i=0;i<WRITER_NO;i++){
        waitpid(pid[i],NULL,0);
    }
    semctl(semid, 0, IPC_RMID); 
    shmdt(shared_memory);
    return 0;
}
