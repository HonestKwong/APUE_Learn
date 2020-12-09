#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



extern char **environ;   //环境变量！

int main(){
    pid_t pid;
    fflush(NULL);

    pid = fork();
    if(pid<0){
        perror("fork()");
        exit(1);

    }
    if(pid == 0){
        execl("/bin/sh", "sh", "-c", "date +%s > bca", NULL);
        perror("execl()");
        exit(1);

    }

    wait(NULL);

    exit(0);
}