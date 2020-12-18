#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>
#include <net/if.h>
#include "proto.h"

#define IPSTRSIZE 40


int main()
{
    int sd;
    struct sockaddr_in laddr, raddr;
    struct msg_st rbuf;
    socklen_t raddr_len;
    char ipstr[IPSTRSIZE];
    
    sd = socket(AF_INET, SOCK_DGRAM, 0/*IPPROTO_UDP*/);
    if(sd<0){
        perror("socket()");
        exit(1);
    }

    struct ip_mreqn mreq;
    inet_pton(AF_INET, MTROUP , &mreq.imr_multiaddr);
    inet_pton(AF_INET, "0.0.0.0" , &mreq.imr_address);
    mreq.imr_ifindex = if_nametoindex("wlp6s0");
    //设置套接字选项,接收方应该加入多播组
    if(setsockopt(sd,IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq))<0){
        perror("setsocketopt()");
        exit(1);
    }

    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(atoi(RCVPORT));
    inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr);
    if(bind(sd, (void *)&laddr, sizeof(laddr))<0){
        perror("bind()");
        exit(1);
    }
    /* !!! */
    raddr_len = sizeof(raddr); 
    while(1){
        recvfrom(sd, &rbuf, sizeof(rbuf), 0, (void*)&raddr, &raddr_len);
        inet_ntop(AF_INET, &raddr.sin_addr, ipstr, IPSTRSIZE);
        printf("---MESSAGE FROM %s:%d ---\n", ipstr, ntohs(raddr.sin_port));
        printf("NAME = %s\n", rbuf.name);
        printf("MATH=%d\n",ntohl(rbuf.math));
        printf("CHINESE=%d\n",ntohl(rbuf.chinese));

    }
    
    
    close(sd);
    
    
    exit(0);
}