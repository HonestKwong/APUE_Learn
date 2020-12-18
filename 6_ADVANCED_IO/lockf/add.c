#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

#define PRONUM 20
#define FNAME "/tmp/out"
#define LINESIZE 1024

static void func_add(void){
    int fd;
    FILE *fp;
    char linebuf[LINESIZE];
    fp = fopen(FNAME, "r+");
    if(fp == NULL){
        perror("fopen() failed");
        exit(1);
    }
    fd = fileno(fp);
    lockf(fd, F_LOCK, 0);
    /****        临界区       ******/
    fgets(linebuf, LINESIZE, fp);
    fseek(fp, 0, SEEK_SET); //定位到文件首?
    fprintf(fp,"%d\n",atoi(linebuf)+1);  //老师是单核的,所以没有出现竞争
    //线程七手八脚打开用一个文件.产生覆盖写,可以用sleep(1)放大故障.
    fflush(fp);
    // sleep(1);
    
    // fclose(fp); //可能造成异常锁
    /****        临界区       ******/
    lockf(fd, F_ULOCK, 0);
    fclose(fp);

    return;
}



int main(){
    pid_t pid;
    int err;
    for(int i=0; i<PRONUM; i++){
        pid = fork();
        if(pid<0){
            perror("fork()");
            exit(1);
        }
        if(pid==0){
            func_add();
            exit(0);
        }
        
    }
    for(int i=0; i<PRONUM; i++){
        wait(NULL);
    }





    exit(0);
}