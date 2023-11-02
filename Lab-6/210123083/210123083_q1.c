#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>

typedef struct my_msg{
	long int msg_type;
	int row;
	int column;
} my_msg;

int main(){
	int msgid;
	key_t key;

	// creating a new key
	key = ftok("msg_send_file.txt", 69);

	// creating the msg queue and storing msgid
	msgid = msgget(key, 0666|IPC_CREAT);

	if(msgid == -1){
		printf("Error, Message Queue not created!");
	}
	
	for(int i=1; i<=256; i++){
		for(int j=1; j<=256; j++){
			my_msg msg1;
			msg1.msg_type = 1;
			msg1.row = i; 
			msg1.column = j;
			
			// sending the message into message queue
			if(msgsnd(msgid, (my_msg *)&msg1, sizeof(msg1), 0) == -1){
				printf("Message could not be sent!!");
			}
			printf("Sender Sent:- (%d, %d)\n", msg1.row, msg1.column);
		}
	}
	sleep(10);
	
	// destroying the message queue
	msgctl(msgid, IPC_RMID, 0);
	
	return 0;
}
