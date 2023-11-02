#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdbool.h>

struct Player{
	int runs;
	int balls;
	char c;
	char bowler[20];
};

int main(){
    // ftok to generate a unique key
    key_t key = ftok("shmfile", 65);

    // shmget returns an identifier in shmid
    // r-4 w-2 x-1 and 6 = 4+2 (r + w)
    int shmid = shmget(key, 70000, 0666);

    // shmat to attach to shared memory
    char *str = (char *) shmat(shmid, (void *)0, 0);

    //printf("Data read from memory: %s\n", str);
    
    char *lines[601];
    
    for(int i=0; i<601; i++){
    	char *line;
    	if(i == 0) line = strtok(str, "\n");
    	else line = strtok(NULL, "\n");
    	
    	lines[i] = malloc(500);
    	strcpy(lines[i], line);
    	
    	//if(line != NULL) printf("%s\n", lines[i]);
    }
    
    struct Player gambhir;
    gambhir.runs = 0;
    gambhir.c = '*';
    gambhir.balls = 0;
    strcpy(gambhir.bowler, "");
    
    struct Player sehwag;
    sehwag.runs = 0;
    sehwag.c = '*';
    sehwag.balls = 0;
    strcpy(sehwag.bowler, "");
    
    struct Player pujara;
    pujara.runs = 0;
    pujara.c = '*';
    pujara.balls = 0;
    strcpy(pujara.bowler, "");
    
    struct Player tendulkar;
    tendulkar.runs = 0;
    tendulkar.c = '*';
    tendulkar.balls = 0;
    strcpy(tendulkar.bowler, "");
    
    struct Player kohli;
    kohli.runs = 0;
    kohli.c = '*';
    kohli.balls = 0;
    strcpy(kohli.bowler, "");
    
    struct Player raina;
    raina.runs = 0;
    raina.c = '*';
    raina.balls = 0;
    strcpy(raina.bowler, "");
    
    struct Player dhoni;
    dhoni.runs = 0;
    dhoni.c = '*';
    dhoni.balls = 0;
    strcpy(dhoni.bowler, "");
    
    for(int i=0; i<601; i++){
    	char *word = strtok(lines[i], " ");
    	//printf("%s\n", word);
    	int j=0;
    	char *bowler = malloc(20);
    	char *batsman = malloc(20);
    	int run = 0;
    	bool wide = false;
    	bool flag = false;
    	while(word != NULL){
    		if(j == 1) {
    			//printf("%s\n", word);
    			strcpy(bowler, word);
    		}
    		else if(j == 3){
    			//printf("%s\n", word);
  			strcpy(batsman, word);
    		}
    		else if(j == 4){
    			//printf("%s\n", word);
    			if(strcmp(word, "no") == 0) run = 0;
    			else if(strcmp(word, "1") == 0) run = 1;
    			else if(strcmp(word, "2") == 0) run = 2;
    			else if(strcmp(word, "3") == 0) run = 3;
    			else if(strcmp(word, "FOUR,") == 0) run = 4;
    			else if(strcmp(word, "SIX,") == 0) run = 6;
    			else if(strcmp(word, "OUT,") == 0) flag = true;
    		}
    		else if(j == 5){
    			if(strcmp(word, "wide,") == 0) {
    				run--;
    				wide = true;
    			}
    			else if(strcmp(word, "bye,") == 0) {
    				run = 0;
    			}
    			else if(strcmp(word, "leg") == 0) {
    				run = 0;
    			}
    		}
    		word = strtok(NULL, " ");
    		j++;
    	}
    	if(strcmp(batsman, "Gambhir,") == 0){
    		gambhir.runs += run;
    		if(!wide) gambhir.balls += 1;
    		if(flag){ 
    			gambhir.c = 'b';
    			strcpy(gambhir.bowler, bowler);
    		}
    	}
    	else if(strcmp(batsman, "Sehwag,") == 0){
    		sehwag.runs += run;
    		if(!wide) sehwag.balls += 1;
    		if(flag){ 
    			sehwag.c = 'b';
    			strcpy(sehwag.bowler, bowler);
    		}
    	}
    	else if(strcmp(batsman, "Pujara,") == 0){
    		pujara.runs += run;
    		if(!wide) pujara.balls += 1;
    		if(flag){ 
    			pujara.c = 'b';
    			strcpy(pujara.bowler, bowler);
    		}
    	}
    	else if(strcmp(batsman, "Tendulkar,") == 0){
    		tendulkar.runs += run;
    		if(!wide) tendulkar.balls += 1;
    		if(flag){ 
    			tendulkar.c = 'b';
    			strcpy(tendulkar.bowler, bowler);
    		}
    	}
    	else if(strcmp(batsman, "Kohli,") == 0){
    		kohli.runs += run;
    		if(!wide) kohli.balls += 1;
    		if(flag){ 
    			kohli.c = 'b';
    			strcpy(kohli.bowler, bowler);
    		}
    	}
    	else if(strcmp(batsman, "Raina,") == 0){
    		raina.runs += run;
    		if(!wide) raina.balls += 1;
    		if(flag){ 
    			raina.c = 'b';
    			strcpy(raina.bowler, bowler);
    		}
    	}
    	else if(strcmp(batsman, "Dhoni,") == 0){
    		dhoni.runs += run;
    		if(!wide) dhoni.balls += 1;
    		if(flag){ 
    			dhoni.c = 'b';
    			strcpy(dhoni.bowler, bowler);
    		}
    	}
    }
    
    printf("Gambhir %c %s %d %d\n", gambhir.c, gambhir.bowler, gambhir.runs, gambhir.balls);
    printf("Sehwag %c %s %d %d\n", sehwag.c, sehwag.bowler, sehwag.runs, sehwag.balls);
    printf("Pujara %c %s %d %d\n", pujara.c, pujara.bowler, pujara.runs, pujara.balls);
    printf("Tendulkar %c %s %d %d\n", tendulkar.c, tendulkar.bowler, tendulkar.runs, tendulkar.balls);
    printf("Kohli %c %s %d %d\n", kohli.c, kohli.bowler, kohli.runs, kohli.balls);
    printf("Raina %c %s %d %d\n", raina.c, raina.bowler, raina.runs, raina.balls);
    printf("Dhoni %c %s %d %d\n", dhoni.c, dhoni.bowler, dhoni.runs, dhoni.balls);
    // detach from the shared memory
    shmdt(str);

    // destroy the shared memory
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
