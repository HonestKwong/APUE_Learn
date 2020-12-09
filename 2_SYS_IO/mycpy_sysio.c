#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFSIZE 1024


int main(int argc, char **argv){
    int sfd, dfd;
    char buf[BUFSIZE];
    int len = 0;
    int ret = 0;
    int pos = 0;
    if(argc<3){
        fprintf(stderr, "Usage....\n");
        exit(1);
    }
    
    sfd = open(argv[1], O_RDONLY);
    if(sfd<0){
        perror("open():sfd");
        exit(1);
    }
    dfd = open(argv[2], O_WRONLY | O_CREAT, O_TRUNC, 0600); //0600是umask？
    if(dfd < 0){
        perror("open():dfd");
        exit(1);
    }

    while(1){
        len = read(sfd, buf, BUFSIZE); //len为读入的字节数
        if(len<0){
            perror("read()");
            break;
        }
        if(len == 0) break;
        pos = 0;
        while(len>0){
            ret = write(dfd, buf+pos, len); // ret为写入的字节数
                if(ret<0)
                {
                    perror("close()");
                    exit(1);
                }
            pos +=ret;
            len -= ret;
        }
    }

    close(dfd);
    close(sfd);
    exit(0);
}