#include <stdio.h>
#include <stdlib.h>
#include "server_conf.h"
#include <proto.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


/*
 * -M   指定多播组
 * -P   指定接收端口
 * -F   前台运行
 * -H   显示帮助
 * -D   指定媒体库位置
 * -I   指定网络设备
 * */

static void printfhelp(void){
    printf("-M   指定多播组\n");
    printf("-P   指定接收端口\n");
    printf("-F   前台运行\n");
    printf("-D   指定媒体库位置\n");
    printf("-I   指定网络设备\n");
    printf("-H   显示帮助\n");
    

}

static daemonize(void){
    pid_t pid;
    int fd;
    pid = fork(); //父进程中的信号掩码是不被继承的
    if(pid<0){
        perror("fork()");
        return -1;
    }
    if(pid>0)     //parent 退出父进程
        exit(0);
    
    fd = open("/dev/null", O_RDWR);
    if(fd<0){
        perror("open()");
        return -2;
    } 

    dup2(fd,0);
    dup2(fd,1);
    dup2(fd,2);

    if(fd > 2)
        close(fd);

    setsid();

    chdir("/");  //指定到合适位置
    
    umask(0);

    return 0;






}

struct  server_conf_st server_conf = 
        {
        .rcvport = DEFAULT_RCVPORT,\
        .mgroup = DEFAULT_MGROUP,\
        .media_dir = DEFAULT_MEDIADIR,\
        .runmode = RUN_DAEMON,\
        .ifname = DEFAULT_IF
        };


int main(int argc, char** argv){
    int c;
    /*命令行分析*/
    while(1){
        c = getopt(argc, argv, "M:P:FD:I:H");
        if(c < 0) break;
        switch ((c))
        {
        case 'M':
            server_conf.mgroup = optarg;
            break;
        case 'P':
            server_conf.rcvport = optarg;
            break;

        case 'F':
            server_conf.runmode = RUN_FOREGROUND;
            break;

        case 'D':
            server_conf.media_dir = optarg;
            break;
        case 'I':
            server_conf.ifname = optarg;
            break;

        case 'H':
            printfhelp();
            exit(0);
            break;
        default:
            abort();
            break;
        }
    }
    /*守护进程的实现*/
    if(server_conf.runmode == RUN_DAEMON){
        if(daemonize()!=0)  //守护进程无法进行
            exit(1);
    }
    else if(server_conf.runmode == RUN_FOREGROUND){
        /*do nothing*/
    }
    else{
        fprintf(stderr, "EINVAL\n");  
        exit(1);
    }

    
    /*SOCKET初始化*/


    /*获取频道信息*/


    /*创建节目单线程*/



    /*创建频道线程*/

    while(1) pause();



    exit(0);
}
