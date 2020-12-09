#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#define FNAME "/tmp/out"

FILE* fp;
static int daemonize(void){
    int fd;
    pid_t pid;
    pid = fork();
    if(pid<0){
        // perror("fork()");
        return -1;
        }
    if(pid>0)
        exit(0);
    fd = open("/dev/null",O_RDWR);
    if(fd<0){
        // perror("open()");
        return -1;
    }
    dup2(fd,0);
    dup2(fd,1);
    dup2(fd,2);
    if(fd>2)
        close(fd);
    setsid();
    chdir("/");
    // umask(0); //觉得不会产生文件了
    return 0;

}

static void deamon_exit(int s){
    //实现可以不一样
    // 例如 if(s == SIGTERM) .....; 容易出现重入！
    fprintf(fp,"deamon exit!\n");  //可以在fp指向的文件中查看到进程顺利退出！
    fclose(fp); //这里执行不到，只能用信号处理函数
    closelog();
    
    exit(0); //还可以执行钩子函数
}

int main(){
    
    

    

    // signal(SIGINT, daemon_exit);  //共用一个信号处理函数可能发生重入！！执行中被打断！
    // signal(SIGQUIT, daemon_exit);
    // signal(SIGTERM, daemon_exit);
    /*********下面是正解！***************/
    struct  sigaction sa;
    sa.sa_handler = deamon_exit;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGINT);  //这是希望要SIGINT时BLOCK掉的信号！
    sigaddset(&sa.sa_mask, SIGTERM);
    sigaddset(&sa.sa_mask, SIGINT); //可以把自己也block住？

    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    sigaction(SIGQUIT, &sa, NULL);

    sigaction(SIGTERM, &sa, NULL);

    openlog("mymdaemon", LOG_PID, LOG_DAEMON); //第二个记录PID，第三个选择来源：守护进程
    if(daemonize()){
       syslog(LOG_ERR, "daemonize() failed!");//1.级别 2.输出内容！
       exit(1);
    }
    else{
        syslog(LOG_INFO, "daemonize() succeded!");//1.级别 2.输出内容！
    }
   fp = fopen(FNAME,"w");
    if(fp == NULL){
        // perror("fpopen()");
        syslog(LOG_ERR, "fopen():%s", strerror(errno));//1.级别 2.输出内容！
        exit(1);
    }

    syslog(LOG_INFO, "%s was opened", FNAME);//1.级别 2.输出内容！
    for(int i=0; ;i++){
        fprintf(fp,"%d\n", i);
        fflush(fp);
        syslog(LOG_DEBUG, "%d was printed", i);
        sleep(1);
    }

    fclose(fp); //这里执行不到，只能用信号处理函数
    closelog();

   exit(0);
}