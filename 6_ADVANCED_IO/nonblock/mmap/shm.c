#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MEMSIZE 1024

int main(){
    char* ptr;
    pid_t pid;
    ptr = mmap(NULL, MEMSIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);
    if(ptr == MAP_FAILED){
        perror("mmap()");
        exit(1);
    }
    pid = fork();
    if(pid<0){
        perror("fork()");
        munmap(ptr, MEMSIZE);  //如果创建成功也要释放内存!
        exit(1);
    }


    if(pid==0)  //child write
    {
        strcpy(ptr, "Hello!");
        munmap(ptr, MEMSIZE); 
        exit(0);
    }
    else  //father read
    {
        wait(NULL);
        puts(ptr);
        munmap(ptr, MEMSIZE); 
        exit(0);
    }
    exit(0);


}