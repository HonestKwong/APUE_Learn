#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char** argv){
    int fd;
    if(argc<2){
        fprintf(stderr,"Usage..\n");
        exit(1);
    }
    fd = open(argv[1], O_WRONLY|O_CREAT|O_TRUNC, 0600);
    if(fd<0){
        perror("open()");
        exit(1);
    }
    lseek(fd, (5LL*1024*1024*1024-1),SEEK_SET);  //每个数据都包含数据类型

    write(fd,"",1);

    exit(0);

}