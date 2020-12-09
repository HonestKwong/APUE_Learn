#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

static void*  func(void* p){
    puts("Thread is working");
    // return NULL; 第一次用的
    pthread_exit(NULL); //会实现线程清理
}

int main(){
    pthread_t tid;
    int err;
    puts("Begin!");

    err = pthread_create(&tid, NULL, func, NULL); //不设置属性直接跟着默认走!填为空!
    if(err){
        fprintf(stderr,"pthread_create failed%s\n",strerror(err));
        exit(1);

    }
    // tid = 0;
    err = pthread_join(tid, NULL);  //等待创建的进程收拾
    if(err){
        fprintf(stderr,"pthread_join failed %s\n",strerror(err));
    }
    puts("End!");
    // pause();
    exit(0);

}