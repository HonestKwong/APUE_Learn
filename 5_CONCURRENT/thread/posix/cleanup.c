#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

static void* cleanup_func(void* p){
    puts(p);
    pthread_exit(NULL);
    
}

static void* func(void* p){
    puts("thread is working");

    pthread_cleanup_push(cleanup_func, "cleanup:1"); //压栈
    pthread_cleanup_push(cleanup_func, "cleanup:2");
    pthread_cleanup_push(cleanup_func, "cleanup:3");
    puts("push over");
    pthread_exit(NULL);
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    
}

int main(){  //一开始提示应该输入while
    puts("Begen!");
    pthread_t tid;
    int err;
    err = pthread_create(&tid, NULL, func, NULL);
    if(err){
        fprintf(stderr, "pthread_create failed %s",strerror(err));
        exit(1);
    }
    
    pthread_join(tid, NULL);


    puts("End!");

    exit(0);
}