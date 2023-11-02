#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>

// function to print the sequence
void print_seq(int n){
	while(n > 1){
		printf("%d ", n);
		if(n%2 == 0) n/=2;
		else n = 3*n+1;
	}
	printf("%d \n", n);
}

int main(int args, char *kwargs[]){
	int n;
	bool loop = false;
	int arr[args];
	if(args < 2) n = 100;
	else if(args == 2) n = atoi(kwargs[1]);
	else{
		loop = true;
		for(int i=0; i<args-1; i++) arr[i] = atoi(kwargs[i+1]);
	}// if args > 2, loop wil run, else only one time the sequence will print
	if(loop == true){
		for(int i=0; i<args-1; i++) print_seq(arr[i]);
	}
	else print_seq(n);
	return 0;
}
