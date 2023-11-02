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
	else if(p1 == 0) // child process 1
	{
		execle("210123083-q1a", "210123083-q1a", NULL, NULL);
	}
	else // parent process
	{
		pid_t p2 = fork();
		if(p2 < 0) {
			printf("Fork Failed\n");
			exit(0);
		}
		else if(p2 == 0) // child process 2
		{
			execl("210123083-q1a", "210123083-q1a", "69", NULL);
		}
		else // parent process
		{
			pid_t p3 = fork();
			if(p3 < 0) {
				printf("Fork Failed\n");
				exit(0);
			}
			else if(p3 == 0) // child process 3
			{
				char *const args[] = {"210123083-q1a", "10", "20", "30", "40", "50", "60", "70", "80", "90", "99", NULL};
				execve("210123083-q1a", args, NULL);
			}
			else // parent process
			{
				wait(NULL);
				wait(NULL);
				wait(NULL);
				
				//sleep(1);
				printf("All child processes completed\n");
			}
		}
	}
	return 0;
}
