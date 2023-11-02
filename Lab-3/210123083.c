#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <errno.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <string.h>

extern int kill(int, int);

void start_daemon(){

    int i, fd0, fd1, fd2;
    pid_t pid;
    struct rlimit rl;   

    // Get maximum number of file descriptors.
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
        printf("can't get file limit");
        exit(1);
    }

    if ((pid = fork()) < 0){
        printf("can't fork");
        exit(1);
    }
    else if (pid != 0) exit(0); // parent

    // Clear file creation mask.
    umask(0);
    
    if ((pid = fork()) < 0){
        printf("can't fork");
        exit(1);
    }
    else if (pid != 0) exit(0); // parent
    setsid();
    FILE *fp = fopen("daemon_id", "w");
    fprintf(fp, "%d" ,getpid());
    fclose(fp);
    /*
        Change the current working directory to the root so
        we won’t prevent file systems from being unmounted.
    */
    if (chdir("/") < 0) {
        printf("can't change directory to /");
        exit(1);
    }
    
    // Close all open file descriptors.
    if (rl.rlim_max == RLIM_INFINITY) rl.rlim_max = 1024;
    for (i = 0; i < rl.rlim_max; i++) close(i);
    
    // Initialize the log file.
    openlog("my_daemon", LOG_PID | LOG_CONS, LOG_DAEMON);

    syslog(LOG_INFO, "Successfully created Daemon!\n");

    while(1){
        pid_t x;
        if((x = fork()) < 0){
            printf("Fork Failed!");
            exit(1);
        }
        if(x == 0){
            int n = getpid();
            while(n > 1){
                syslog(LOG_INFO, "%d ", n);
                if(n%2 == 0) n/=2;
                else n = 3*n+1;
            }
            syslog(LOG_INFO, "%d\n", n);
            exit(0);
        }
        if(x > 0){
            wait(NULL);
            sleep(20);
        }
    }
}

void stop_daemon(){
    FILE *fp = fopen("daemon_id", "r");
    int _pid_;
    fscanf(fp, "%d", &_pid_);
    if(_pid_ > 0) kill(_pid_, SIGTERM);
    openlog("my_daemon", LOG_PID | LOG_CONS, LOG_DAEMON);
    syslog(LOG_INFO, "Daemon Process Killed");
}

int main(int argv, char **argc){
    if(argv < 2) return 0;
    if(strcmp(argc[1], "start") == 0) start_daemon();
    if(strcmp(argc[1], "stop") == 0) stop_daemon();

    return 0;
}