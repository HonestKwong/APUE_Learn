#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#define LEFT 30000000
#define RIGHT 30000200
int main(){
    int i, j, mark;
    pid_t pid;
    for(i = LEFT;i<=RIGHT; i++){
        pid = fork();
        if(pid<0){
            perror("fork()");
            exit(1);

        }
        if(pid == 0){
            mark = 1;
            for(j=2; j<i/2; j++){
                if(i%j == 0){
                    mark = 0;
                    break;
                }
            }
            if(mark){
                printf("%d is a primer\n", i);
            }
            // sleep(1000);  //父进程结束，子进程变孤儿！
            exit(0); //这里必须结束子进程！！！，
            //这样子进程会不断创建子进程，
            //可能引起不堪设想的结果
        }
    }
    // int st;
    for(i = LEFT; i<=RIGHT; i++) wait(NULL); //wait是收回来的状态
    // sleep(1000);  //父进程未结束，子进程变僵尸
    exit(0);

}