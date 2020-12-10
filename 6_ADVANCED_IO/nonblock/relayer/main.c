#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include "relayer.h"

#define TTY1 "/dev/tty11"
#define TTY2 "/dev/tty12"
#define TTY3 "/dev/tty10"
#define TTY4 "/dev/tty9"




int main(){
    int job1, job2;
    int fd1,fd2,fd3,fd4;
    fd1 = open(TTY1, O_RDWR);
    if(fd1<0){
        perror("open()");
        exit(1);
    }
    write(fd1, "TTY1\n",5);
    fd2 = open(TTY2, O_RDWR);
    if(fd2<0){
        perror("open()");
        exit(1);
    }
    write(fd2, "TTY2\n",5);

    job1 = rel_addjob(fd1, fd2);
    if(job1<0){
        fprintf(stderr,"rel_addjob() failed:%s\n",strerror(-job1));
    }

    fd3 = open(TTY3, O_RDWR);
    if(fd3<0){
        perror("open()");
        exit(1);
    }
    write(fd3, "TTY1\n",5);
    fd4 = open(TTY4, O_RDWR);
    if(fd4<0){
        perror("open()");
        exit(1);
    }
    write(fd4, "TTY2\n",5);

    job2 = rel_addjob(fd3, fd4);
    if(job2<0){
        fprintf(stderr,"rel_addjob() failed:%s\n",strerror(-job2));
    }

    while(1) pause();

    close(fd1);
    close(fd2);
    close(fd3);
    close(fd4);
    exit(0);
}