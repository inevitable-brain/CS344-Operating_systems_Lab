There are 3 codes repo.c, reader.c and writer.c

First run repo.c -> it'll keep running for 1 minute(can be changed in sleep)

Compile writer.c and run in terminal. It has 2 child processes writing to shared memory. Number of children can be changed by changing WRITER_NO in code.
Compile reader.c and run in different terminals to simulate readers.

Writer and readers will show shmget or semop errors after 1m minute as repository.c will delete the semaphores and shared memory.
A temporary file tem.txt is generated to store the readers count.

f): In write.c, inside each child process, we call semaphore operations wait and signal on writer semaphore, which ensures that at a particular time, only one process gets to write in the shared memory segment.

g): If there is no other reader when a reader wants to read, read count will become 1 and reader will wait for semaphore which is currently held by writer. If there were already some readers when a new reader enters, then there will be a first reader which will be waiting for the write lock and so will have held the read lock also. Read lock is released only when write lock is obtained. So, this reader will not be able to enter its read section.

h): When a reader is in the read section, there are two cases.
	Case 1: It holds the write lock. The other readers can enter the read section as the read lock is free and they don't have to wait for the write lock as they are not the first writer.
	Case 2: If it does not hold the writers lock, some other reader is holding the writers lock and new readers won't have to wait for read or write lock.
This ensures that when a reader reads, other readers are also able to read. 
