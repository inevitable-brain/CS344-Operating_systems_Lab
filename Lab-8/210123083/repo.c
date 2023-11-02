#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include<string.h>

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

int main()
{
    key_t key = ftok("repository.c", 0);
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0644);

    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Attach the shared memory
    char* shared_memory = (char*)shmat(shmid, NULL, 0);

    // Initialize the shared memory with "Hello, world!"
    strcpy(shared_memory, "#include<stdio.h>\nint main(){\n\tprintf(\"Hello World!\");\n}\n");

    key_t semkey = ftok("writer.c", 'a');
    int semid = semget(semkey, 1, IPC_CREAT | 0644);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }

    // Initialize the semaphore to 1 for exclusive access
    union semun sem_arg;
    sem_arg.val = 1;
    semctl(semid, 0, SETVAL, sem_arg);

    key_t readkey = ftok("reader.c", 'a');
    int readid=semget(readkey, 1, IPC_CREAT | 0644);
    if (readid== -1) {
        perror("semget");
        exit(1);
    }

    // Initialize the semaphore to 1 for exclusive access
    semctl(readid, 0, SETVAL, sem_arg);

    sleep(40);
    semctl(semid, 0, IPC_RMID); 
    semctl(readid, 0, IPC_RMID); 
    // Detach the shared memory
    shmdt(shared_memory);
}