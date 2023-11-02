#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>

void print_seq(int n){
	while(n > 1){
		printf("%d ", n);
		if(n%2 == 0) n/=2;
		else n = 3*n+1;
	}
	printf("%d \n", n);
}

int main(int args, char *kwargs[]){
	int n=0;
	bool loop = false;
	if(args > 1){
		printf("You are not allowed to pass any command line arguments.\n");
		return 0;
	}
	char *input = getenv("n");
	// if env variable is unset, default n = 100 else depending on 1 or more values the sequence will print.
	if(input == NULL) n = 100;
	else{
		int i = 0;
		while(input[i]){
			if(input[i] != ' ') n = n*10 + (input[i] - '0');
			else {
				print_seq(n);
				n=0;
			}
			i++;
		}
	}
	// last or base case
	print_seq(n);
	return 0;
}
