#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



extern char **environ;   //环境变量！

int main(){
    pid_t pid;
    puts("Begin!");
    fflush(NULL);

    pid = fork();
    if(pid<0){
        perror("fork()");
        exit(1);

    }
    if(pid == 0){
        execl("/bin/date", "date", "+%s", NULL);
        perror("execl()");
        exit(1);

    }

    wait(NULL);

    puts("End!");

    exit(0);
}