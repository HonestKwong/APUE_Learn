#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static volatile int loop = 1;  //不要轻信内存中的值？ 去数值真正的空间取数值！
static void alrm_handler(int s){
    loop = 0;
}

int main(){
    signal(SIGALRM,alrm_handler);
    alarm(5);
    int64_t count = 0;
    

    while(loop)
        count++;
    printf("%lld\n", count);

    exit(0);
}