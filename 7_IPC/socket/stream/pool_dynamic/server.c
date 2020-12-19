#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <time.h>
#include "proto.h"
#include <wait.h>
#include <unistd.h>  
#include <signal.h>
#include <sys/mman.h>
#include <errno.h>


// #define IPSTRSIZE 40
// #define BUFSIZE 1024
// #define PROCNUM 4   //四个线程限制太死

#define MINSPARESERVER 5
#define MAXSPARESERVER 10
#define MAXCLIENTS 20
#define IPSTRSIZE 40
#define LINEBUFSIZE 80 

#define SIG_NOTIFY SIGUSR2

enum{
    STATE_IDEL = 0,
    STATE_BUSY
};
struct server_st{
    pid_t pid;
    int state;
    /* int reuse Appach中使用运行一段时间让其消亡*/
};

static struct server_st *serverpool;

static int idle_count = 0, busy_count = 0;

static int sd;

static void usr2_handler(int s){
    return;
}

static void server_job(int pos){
    pid_t ppid;
    struct sockaddr_in raddr;
    socklen_t raddr_len;
    int client_sd;
    time_t stamp;
    // char* linebuf;
    char ipstr[IPSTRSIZE];
    char linebuf[LINEBUFSIZE];
    int len;

    ppid = getppid();
    while(1){
        serverpool[pos].state = STATE_IDEL;
        kill(ppid, SIG_NOTIFY);
        client_sd = accept(sd, (void *)&raddr, &raddr_len);
        if(client_sd < 0){
            if(errno != EINTR || errno != EAGAIN)
            {
                perror("accept()");
                exit(1);
            }
        }
        serverpool[pos].state=STATE_BUSY;
        // busy_count++; 不知道要不要
        kill(ppid, SIG_NOTIFY);
        inet_ntop(AF_INET, &raddr.sin_addr, ipstr, IPSTRSIZE);
        // printf("[%d]client:%s:%d\n",getpid(), ipstr, ntohs(raddr.sin_port));
        stamp = time(NULL);
        len = snprintf(linebuf, LINEBUFSIZE, FMT_STAMP, stamp); //写到一个字符串当中去
        send(client_sd, linebuf, len ,0);
        sleep(5);
        close(client_sd);
        // busy_count--;
    }
}

static int add_1_server(void){
    if((idle_count + busy_count) > MAXCLIENTS) return -1;
    int slot;
    pid_t pid;
    for(slot = 0; slot<MAXCLIENTS;slot++){
        if(serverpool[slot].pid == -1)
            break;
    }
    serverpool[slot].state = STATE_IDEL;
    pid = fork();
    if(pid < 0) {
        perror("fork()");
        exit(1);
    }
    if(pid == 0) //子进程干活去
    {
        server_job(slot);
        exit(0);
    }
    else{
        serverpool[slot].pid = pid;
        idle_count++;
    }

}

static int del_1_server(){
    if(idle_count == 0) return -1;
    int i;
    for(i =0; i< MAXCLIENTS;i++){
        if(serverpool[i].pid != -1 && serverpool[i].state == STATE_IDEL){
            kill(serverpool[i].pid, SIGTERM);
            serverpool[i].pid = -1;
            idle_count--;
            break;
        }
    }
    return 0;
}

static int scan_pool(void){
    int i;
    int idle = 0;
    int busy = 0;
    for(i = 0; i<MAXCLIENTS; i++){
        if(serverpool[i].pid == -1) continue;
        if(kill(serverpool[i].pid,0)){  //发送零，检测进程式发展存在！
            serverpool[i].pid = -1;
            continue;
        }
        if(serverpool[i].state == STATE_IDEL) idle++;
        else if(serverpool[i].state == STATE_BUSY)
            busy++;
            else{
                fprintf(stderr, "Unknown state.\n");
                abort(); //给自己发送一个
            }
    }
    idle_count = idle;
    busy_count = busy;
    return 0;
}



int main(){

    struct  sigaction sa,osa;
    int val = 1;

    struct  sockaddr_in laddr;
    sigset_t set, oset;
    int i;
    

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NOCLDWAIT;
    sigaction(SIGCHLD, &sa, &osa);
    sa.sa_handler = usr2_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIG_NOTIFY, &sa, &osa);

    sigemptyset(&set);
    sigaddset(&set, SIG_NOTIFY);
    sigprocmask(SIG_BLOCK, &set, &oset);

    //开辟一块20个server_st空间
    serverpool = mmap(NULL, sizeof(struct  server_st) *MAXCLIENTS,PROT_WRITE|PROT_READ, MAP_SHARED|MAP_ANONYMOUS, -1,0);
    if(serverpool == MAP_FAILED)
    {
        perror("mmap()");
        exit(1);
    }

    for(i = 0; i < MAXCLIENTS; i++){
        serverpool[i].pid = -1;
    }
    

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd<0){
        perror("socket");
        exit(1);
    }

    if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR,&val, sizeof(val))<0)
    {
        perror("setsocketopt()");
        exit(1);
    }
    laddr.sin_family=AF_INET;
    laddr.sin_port = htons(atoi(SERVERPORT));  //主机序向网络序的转换
    inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr);
    if(bind(sd,(void*)&laddr, sizeof(laddr))<0)
    {
        perror("bind()");
        exit(1);
    }


    if(listen(sd, 100)<0){
        perror("listen()");
        exit(1);
    }

    for(i = 0; i< MINSPARESERVER; i++){
        add_1_server();
        // fork();
    }

    while(1){
        sigsuspend(&oset);
        scan_pool();
        if(idle_count > MAXSPARESERVER){
            for(i=0; i<(idle_count-MAXSPARESERVER); i++)
                del_1_server();
        }
        else if(idle_count < MINSPARESERVER){
            for(i=0; i<(MINSPARESERVER-idle_count); i++)
                add_1_server();
        }
        //printf the pool
        for(i = 0; i<MAXCLIENTS; i++){
            if(serverpool[i].pid == -1) putchar(' ');
            else if(serverpool[i].state == STATE_IDEL) putchar('.');
            else putchar('x');
        }
        putchar('\n');
        //printf pool
        //contrl the pool
    }

    sigprocmask(SIG_SETMASK, &oset, NULL);
    exit(0);

 
}

