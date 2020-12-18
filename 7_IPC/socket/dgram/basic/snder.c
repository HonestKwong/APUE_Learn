#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>
#include <string.h>
#include "proto.h"

int main(int argc, char* argv[]){
    if (argc<2){
        fprintf(stderr, "Usage...\n");
        exit(1);
    }
    int sd;
    sd = socket(AF_INET, SOCK_DGRAM, 0);

    if(sd<0){
        perror("socket()");
        exit(1);
    }
    struct msg_st sbuf; 
    struct sockaddr_in raddr; 
    // bind();
    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(atoi(RCVPORT));
    inet_pton(AF_INET,argv[1], &raddr.sin_addr);
    memset(&sbuf, '\0', sizeof(sbuf));  //这句话可以防止没用的空间泄漏,全部清空
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