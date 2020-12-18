#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <time.h>
#define BUFSIZE 1024

int main(int argc, char* argv[]){
    int sd;
    struct sockaddr_in  raddr;
    long long stamp;
    FILE* fp;
    char rbuf[BUFSIZE];
    if(argc < 2){
        fprintf(stderr,"Usage...\n");
        exit(1);
    }
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd<0){
        perror("socket()");
        exit(1);
    }


    raddr.sin_family = AF_INET;
    raddr.sin_port = htons(80); //这个函数到底干啥用
    inet_pton(AF_INET,argv[1], &raddr.sin_addr);
    if(connect(sd, (void*)&raddr, sizeof(raddr))<0)
    {
        perror("connect");
        exit(1);
    }
    
    fp = fdopen(sd,"r+");
    if(fp == NULL){
        perror("fdopen()");
        exit(1);

    }

    fprintf(fp, "GET /test/jpg\r\n\r\n");
    fflush(fp);
    while(1){
        int len = fread(rbuf, 1, BUFSIZE, fp);
        if(len <= 0) break;
        fwrite(rbuf, 1,len,stdout);

    }

    fclose(fp);

    exit(0);
}