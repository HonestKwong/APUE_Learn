#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define THRNUM 4
static pthread_mutex_t mut[THRNUM];

static int next(int a){
    if(a<3) return a+1;
    return 0;
}

static void* thr_func(void* p){
    int c = 'a'+(int)p;
    while(1){
        pthread_mutex_lock(mut+(int)p);
        write(1,&c,1);
        pthread_mutex_unlock(mut+next((int)p));
    }
    pthread_exit(NULL);
}


int main(){
    pthread_t tid[THRNUM];
    int err;
    for(int i=0; i< THRNUM; i++){
        pthread_mutex_init(mut+i,NULL);
        pthread_mutex_lock(mut+i);
        err = pthread_create(tid+i, NULL, thr_func, (void*)i);
        if(err){
            fprintf(stderr, "pthread_create():%s\n",strerror(err));
            exit(1);
        }
    }
    pthread_mutex_unlock(mut);

    alarm(5);

    for(int i=0; i< THRNUM; i++){
    pthread_join(tid[i],NULL);
    pthread_mutex_destroy(mut+i);
    }





    exit(0);
}