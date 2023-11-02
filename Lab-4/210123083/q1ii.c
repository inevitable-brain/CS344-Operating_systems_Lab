#include <stdio.h>
#include <string.h>

void print_seq(int n){
	while(n > 1){
		printf("%d ", n);
		if(n%2 == 0) n/=2;
		else n = 3*n+1;
	}
	printf("%d\n", n);
}

int main(int args, char *kwargs[]){
	int n = -1;
	while(n!=1){
		scanf("%d", &n);
		print_seq(n);
	}
	return 0;
}
