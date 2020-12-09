#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define FNAME "/tmp/out"

int main(){
    int fd;
    // close(1);
    fd = open(FNAME, O_WRONLY|O_CREAT|O_TRUNC,0600);
    if(fd<0){
        perror("open()");
        exit(1);
    }
    // close(1);
    // dup(fd);
    // close(fd);
    dp2(fd,1);  //原子操作，不会出错即使fd为1
    if(fd != 1)
        close(fd);

/********************/
    puts("hello!");
    // 应该重定位fd=1 到 stdout到设备
    exit(0);
}