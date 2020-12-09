#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N 3

#define LEFT 30000000
#define RIGHT 30000200
int main(){
    int i, j, mark, n;
    pid_t pid;

    for(n = 0; n< N;n++){
        pid = fork();
        if(pid<0){
            perror("fork()");
            exit(1);
        }
        if(pid == 0){
            for(i = LEFT+n;i<=RIGHT; i+=N){

                mark = 1;
                for(j=2; j<i/2; j++){
                    if(i%j == 0){
                        mark = 0;
                        break;
                    }
                }
                if(mark)
                    printf("[%d]%d is a primer\n", n,i);
                }       
                exit(0);
        }
    }

    // int st;
    for(n = 0; n<=N; n++) wait(NULL); //wait是收回来的状态
    // sleep(1000);  //父进程未结束，子进程变僵尸
    exit(0);

}