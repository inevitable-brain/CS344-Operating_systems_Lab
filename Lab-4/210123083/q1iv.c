#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	char c[400];
	fgets(c, sizeof(c), stdin);
	//printf("%s\n", c);
	//printf("%ld", sizeof(c));
	int x = sizeof(c);
	
	if(strcmp(c, "quit") == 0) return 0;
	
	char c1[200], c2[200], c3[200];
	int i=0, j=0;
	while(i < x && c[i] != '|'){
		c1[i] = c[i];
		i++;
	}
	c1[i-1] = '\0';
	i+=2;
	while(i < x && c[i] != '|'){
		c2[j] = c[i];
		i++;
		j++;
	}
	c2[j-1] = '\0';
	i+=2;
	j=0;
	while(i < x){
		if(c[i] != '\n') c3[j] = c[i];
		i++;
		j++;
	}
	c3[j] = '\0';
	
	printf("c1: %s\nc2: %s\nc3: %s\n", c1, c2, c3);
	
	int fd[2];
	
	if(pipe(fd) < 0){
		printf("Pipe failed!");
		return 1;
	}
	pid_t pid = fork();
	
	if(pid < 0){
		printf("Fork Failed!");
		return 1;
	}
	if(pid == 0)// child
	{
		int fd2[2];
		
		if(pipe(fd2) < 0){
			printf("Pipe failed!");
			return 1;
		}
		
		pid_t pid2 = fork();
		
		if(pid2 < 0){
			printf("Fork Failed!");
			return 1;
		}
		if(pid2 == 0)// child
		{
			close(fd2[1]);
			dup2(fd2[0], STDIN_FILENO);
			close(fd2[0]);
			
			execle(c3, c3, NULL, NULL);
		}
		else // parent
		{
			close(fd[1]); close(fd2[0]);
			dup2(fd[0], STDIN_FILENO);
			dup2(fd2[1], STDOUT_FILENO);
			close(fd[0]); close(fd2[1]);
			
			execle(c2, c2, NULL, NULL);
		}
	}
	else // parent
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		execle(c1, c1, NULL, NULL);
	}
}
