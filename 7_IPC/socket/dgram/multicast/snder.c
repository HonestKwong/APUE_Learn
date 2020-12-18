#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>
#include <string.h>
#include <net/if.h>


#include "proto.h"

int main(int argc, char* argv[]){

    int sd;
    sd = socket(AF_INET, SOCK_DGRAM, 0);

    if(sd<0){
        perror("socket()");
        exit(1);
    }
    struct  ip_mreqn mreq;
    inet_pton(AF_INET, MTROUP, &mreq.imr_multiaddr);
    inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address);
    mreq.imr_ifindex = if_nametoindex("wlp6s0");


    //设置套接字选项,发送方应该创建多播组
    if(setsockopt(sd, IPPROTO_IP,IP_MULTICAST_IF, &mreq, sizeof(mreq))<0){
        perror("setsocketopt()");
        exit(1);
    }

    struct msg_st sbuf; 
    struct sockaddr_in raddr; 
    // bind();
    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(atoi(RCVPORT));
    inet_pton(AF_INET,MTROUP, &raddr.sin_addr);
    memset(&sbuf, '\0', sizeof(sbuf));
    strcpy(sbuf.name, "Alan");
    sbuf.chinese = htonl(rand()%100);
    sbuf.math = htonl(rand()%100);
    
    if(sendto(sd,&sbuf, sizeof(sbuf), 0,(void *)&raddr, sizeof(raddr))<0)
    {
        perror("sendto()");
        exit(1);
    }
    puts("OK!");
    close(sd);


    exit(0);
}