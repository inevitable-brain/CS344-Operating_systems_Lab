#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>

int main(int args, char *kwargs[]){
	if(args > 1){
		printf("You are not allowed to pass any command line arguments.\n");
		return 0;
	}
	pid_t p1 = fork();
	if(p1 < 0) {
		printf("Fork Failed\n");
		exit(0);
	}
	else if(p1 == 0) // child process
	{
		execle("210123083-q1b", "210123083-q1b", NULL, NULL);
	}
	else // parent process
	{
		wait(NULL);
		pid_t p2 = fork();
		if(p2 < 0) {
			printf("Fork Failed\n");
			exit(0);
		}
		else if(p2 == 0) // child process
		{
			setenv("n", "69", 0);
			execl("210123083-q1b", "210123083-q1b", NULL);
		}
		else // parent process
		{
			wait(NULL);
			pid_t p3 = fork();
			if(p3 < 0) {
				printf("Fork Failed\n");
				exit(0);
			}
			else if(p3 == 0) // child process
			{
				setenv("n", "10 20 30 40 50 60 70 80 90 99", 1);
				char * const arg[] = {"210123083-q1b", NULL};
				execve("210123083-q1b", arg, NULL);
				// In this case, the output is being generated with environment variable n to be unset(NULL). I don't know the exact reason, but it maybe because according to the manual of execve command, this command causes the program that is currently being run by the calling process  to  be  re‐placed  with  a  new  program,  with newly initialized stack, heap, and (initialized and uninitialized) data segments.
			}
			else // parent process
			{
				wait(NULL);
				//sleep(1);
				printf("All child processes completed\n");
			}
		}
	}
	return 0;
}
