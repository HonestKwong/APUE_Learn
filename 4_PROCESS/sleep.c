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
        execl("/bin/sleep", "sleep", "100", NULL); //这里argv[0] 看上去是写了别名，低级木马的隐藏方式！
        perror("execl()");
        exit(1);

    }

    wait(NULL);

    puts("End!");

    exit(0);
}