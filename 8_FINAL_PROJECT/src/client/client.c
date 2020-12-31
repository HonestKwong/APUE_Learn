#include <stdio.h>
#include <stdlib.h>

// #include <proto.h>
#include <proto.h>
#include "client.h"

#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>
#include <net/if.h>
#include <wait.h>
#include <errno.h>



/**
* -M --mgroup  指定多播组
* -P --port  指定接受端口
* -p --player  指定播放器
* -H --help  显示帮助
**/

struct client_conf_st client_conf ={\
    .rcvport = DEFAULT_RCVPORT,\
    .mgroup = DEFAULT_MGROUP,
    .player_cmd = DEFAULT_PLAYERCMD
};


static void printfhelp(void){
    printf("-P --port    指定接收端口\n");
    printf("-M --mgroup  指定多播组\n");
    printf("-p --player  指定播放器命令行\n");
    printf("-H --help    打印帮助信息\n");



}

static ssize_t writen(int fd, const uint8_t* buf, size_t len){
    size_t pos = 0;
    int ret;
    while(len > 0)
    {    
        ret = write(fd,buf + pos, len);
        if(ret<0){
            if(errno == EINTR) continue;
            perror("write()");
            return -1;
        }
        len -= ret;
        pos += ret;
    }
    return pos;
}

int main(int argc, char** argv){
    int c;
    int index = 0;
    int sd;
    struct ip_mreqn mreq;
    struct sockaddr_in laddr, serveraddr,raddr;
    socklen_t serveraddr_len, raddr_len;
    int val = 1;
    int pd[2];
    pid_t pid;
    int chosenid;
    int ret;
    int len;


    
/* 初始化 
*   级别： 默认值< 配置文件< 环境变量< 命令行参数
*   
*
**/
struct option argarr[]= {{"port", 1, NULL, 'P'}, \
    {"mgroup", 1, NULL, 'M'}, \
    {"player", 1, NULL, 'p'}, \
    {"help", 0, NULL, 'H'},\
    {NULL, 0, NULL, 0}
    };




    while(1){
        c = getopt_long(argc, argv, "P:M:p:H", argarr, &index);  //:带传参选项
        if(c<0) break;
        switch (c)
        {
        case 'P':
            client_conf.rcvport = optarg;

            break;
        case 'M':
            client_conf.mgroup = optarg;

            break;
        case 'p':
            client_conf.player_cmd = optarg;
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
    
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sd<0){
        perror("socket()");
        exit(1);
    }
    // int val = 1;
    inet_pton(AF_INET, client_conf.mgroup, &mreq.imr_multiaddr);
    /*if error*/
    inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address);
    mreq.imr_ifindex = if_nametoindex("wlp6s0");
    if(setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq))<0) //设置网络套接字选项   查看man 7 ip  Socket options
    {
        perror("setsockopt()");
        exit(1);
    }

    val = 1;
    if(setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, &val, sizeof(val))<0)
    {
        perror("setsockopt()");
        exit(1);
    }
    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(atoi(client_conf.rcvport));
    inet_pton(AF_INET, "0.0.0.0",&laddr.sin_addr);
    if(bind(sd, (void*)&laddr, sizeof(laddr))<0)  //绑定网络
    {
        perror("bind()");
        exit(1);
    }


    if(pipe(pd)<0)
    {
        perror("pipe()");
        exit(1);
    }
    pid = fork();
    if(pid<0){
        perror("fork()");
        exit(1);
    }
    if(pid == 0){
        //子进程:调用解码器
        close(sd);
        close(pd[1]); //0为读端, 1为写端
        dup2(pd[0], 0);  //将管道的读端,作为子进程的输入
        if(pd[0] > 0) close(pd[0]);
        execl("/bin/sh", "sh", "-c", client_conf.player_cmd, NULL); // 空值指示结束
        perror("execl()");
        exit(1);


    }
       //父进程:从网络上收包, 发送给子进程
       //收节目单
       //选择频道
       //收频道包,发送给子进程

    struct msg_list_st *msg_list;
    msg_list = malloc(MSG_LIST_MAX);
    if(msg_list == NULL){
        perror("malloc()");
        exit(1);
    }
    serveraddr_len = sizeof(serveraddr);
    while(1){
        len = recvfrom(sd, msg_list, MSG_LIST_MAX, 0, (void*)&serveraddr,&serveraddr_len);

        if(len < sizeof(struct msg_list_st)){
            fprintf(stderr, "message is too small. \n");
            continue;
        }
        if(msg_list->chnid != LISTCHNID){
            fprintf(stderr, "chnid is not match.\n");
            continue;
        }
        break;
    }

    // 打印节目单并选择频道
    struct msg_listentry_st *pos;

    for(pos = msg_list->entry; (char*)pos < ((char*)msg_list+len) ; pos = (void*)((char*)pos + ntohs(pos->len))){ //char* 一次向后移动一个字节
        printf("channel %d: %s\n",pos->chnid,pos->desc);
    }

    free(msg_list); //打印完可以free掉
    puts("Please input:");
    while(1){
        ret = scanf("%d" ,&chosenid);
        if(ret != 1) exit(1);
        else break;
    }

    fprintf(stdout,"chosenid = %d\n", ret);

    struct msg_channel_st *msg_channel;
    msg_channel = malloc(MSG_CHANNEL_MAX);
    if(msg_channel == NULL){
        perror("malloc()");
        exit(1);
    }

    raddr_len = sizeof(raddr);
    while(1){
        len = recvfrom(sd, msg_channel, MSG_CHANNEL_MAX, 0, (void*)&raddr, &raddr_len);
        printf("len:%d\n", len);
        if(raddr.sin_addr.s_addr != serveraddr.sin_addr.s_addr\
            ||raddr.sin_port != serveraddr.sin_port)    //man 7 ip
        {
            fprintf(stderr, "Ignore: address not match. \n");
            continue;
        }
        if(len < sizeof(struct msg_channel_st)){
            fprintf(stderr, "Ignore: message too small. \n");
            continue;
        }
        if(msg_channel->chnid == chosenid){
            fprintf(stdout, "Accept: accepted msg:%d recieved.\n", msg_channel->chnid);
            if(writen(pd[1], msg_channel->data, len-sizeof(chnid_t))<0)  //writen: 保证写完
                exit(1);
        }


    }
    free(msg_channel);
    close(sd);
    exit(0);
    
    

    







    


    //子进程，调用解码器 exec

    //父进程：从网络上收包，发送给子进程


    // exit(0);
}