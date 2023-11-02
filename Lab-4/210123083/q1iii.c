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
	
	// I am assuming that the input from standard input contains one "|" (pipe) and input is of the form <cmd1> | <cmd2> (spaces are mandatory).
	
	char left[200], right[200];
	int i=0, j=0;
	while(i < x && c[i] != '|'){
		left[i] = c[i];
		i++;
	}
	left[i-1] = '\0';
	i+=2;
	while(i < x){
		if(c[i] != '\n')right[j] = c[i];
		i++;
		j++;
	}
	right[j] = '\0';
	
	char *left_side[100];
	i=0; j=0;
	while(j < strlen(left)){
		left_side[i] = malloc(100);
		int k=0;
		while(j < strlen(left) && left[j] != ' '){
			left_side[i][k] = left[j];
			k++; j++;
		}
		left_side[i][k] = '\0';
		i++; j++;
	}
	left_side[i] = NULL;
	
	printf("left hand of the pipe: %s\nright hand of the pipe: %s\n", left, right);
	
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
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		execle(right, right, NULL, NULL);
	}
	else // parent
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		execve(left_side[0], left_side, NULL);
	}
}
