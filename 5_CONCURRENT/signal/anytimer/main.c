#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include "anytimer.h"

static void f2(void* p){
    printf("f2():%s\n",p);
}

static void f1(void* p){
    printf("f1():%s\n",p);
}


int main(int argc, char **argv){ //Begin!End
    puts("Begin!");
    int job1,job2,job3;
    // job1 = at_addjob(5,f1,"aaa");
    job1 = at_addjob_repeat(2,f1,"aaa");

    if(job1<0){
        fprintf(stderr, "Job1 add%s\n", -job1);
        exit(1);
    }
#if 0
    at_addjob(2,f2,"bbb");
    at_addjob(7,f1,"ccc");
    puts("End!");
#endif
    while(1){
        write(1,".",1);
        sleep(1);
    }
        // pause();

}