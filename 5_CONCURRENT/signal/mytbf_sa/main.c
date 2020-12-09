#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include "mytbf.h"

#define CPS 10
#define BUFSIZE 1024
#define BURST 100


int main(int argc, char **argv){
    int sfd, dfd = 1;
    char buf[BUFSIZE];
    int len = 0;
    int ret = 0;
    int pos = 0;
    mytbf_t* tbf;
    int size = 0;
    if(argc<2){
        fprintf(stderr, "Usage....\n");
        exit(1);
    }
    tbf = mytbf_init(CPS, BURST); //初始化
    if(tbf == NULL){
        fprintf(stderr,"mytbf_init()failed!\n");
        exit(1);
    }

    do{
        sfd = open(argv[1], O_RDONLY);
        if(sfd<0){
            if(errno != EINTR){
                perror("open():sfd");
                exit(1);
            }
        }
    }while(sfd<0);


    while(1){
        size = mytbf_fetchtoken(tbf, BUFSIZE);
        if(size < 0){
            fprintf(stderr, "mytbf_fetchtoken():%s\n", strerror(-size));
            exit(1);
        }
        
        // len = read(sfd, buf, BUFSIZE); //len为读入的字节数
        // if(len<0){
        //     if(errno == EINTR)
        //         continue;
        //     perror("read()");
        //     break;
        // }          //这里李老师讲的好像有一点点问题

        while((len = read(sfd, buf, size))<0){ //len为读入的字节数,这里read卡住会被打断，令牌桶的实现
            if(errno == EINTR)
                continue;
            perror("read()");
            break;
        }

        if(len == 0) break;
        pos = 0;
        if(size-len > 0){
            mytbf_returntoken(tbf, size-len);
        }
        while(len>0){
            ret = write(dfd, buf+pos, len); // ret为写入的字节数
                if(ret<0)
                {
                    if(errno == EINTR)
                        continue;
                    perror("write()");
                    exit(1);
                }
            pos +=ret;
            len -= ret;
        }
        // sleep(1);
    }

    mytbf_destroy(tbf);
    close(sfd);
    exit(0);
}