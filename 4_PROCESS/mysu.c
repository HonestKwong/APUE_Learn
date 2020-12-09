#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



extern char **environ;   //环境变量！

int main(int argc, char** argv){
    pid_t pid;
    if(argc < 3){
       fprintf(stderr,"Usage...\n");
       exit(1); 
    }
    pid = fork();
    if(pid<0){
        perror("fork()");
        exit(1);
    }
    if(pid == 0){
        setuid(atoi(argv[1])); //设置用户
        execvp(argv[2],argv+2); //第二个
        perror("execvp()");
        exit(1);
    }
    wait(NULL);

    exit(0);
}