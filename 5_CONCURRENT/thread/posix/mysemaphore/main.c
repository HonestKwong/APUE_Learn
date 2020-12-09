#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "mysem.h"



#define LEFT 30000000
#define RIGHT 30000200
#define THRNUM (RIGHT-LEFT+1)
#define  N    4
static mysem_t* sem = NULL;

static void* thr_prime(void* p);

int main(){
    int i, err;
    pthread_t tid[THRNUM];

    sem = mysem_init(N);
    printf("&sem:%d",(int)sem);
    if(sem == NULL){
        fprintf(stderr,"mysem_init() failed!\n");
        exit(1);
    }
    for(i = LEFT;i<=RIGHT; i++){
        mysem_sub(sem,1);
        err = pthread_create(tid+(i-LEFT), NULL, thr_prime, (void*)i); //最垃圾的方式
        if(err){
            fprintf(stderr, "pthread_create failed %s", strerror(err));
            exit(1);
        }
    }
    for(i = LEFT;i<=RIGHT; i++)
        pthread_join(tid[i-LEFT],NULL);
    
    mysem_destroy(sem);
    exit(0);
}

static void* thr_prime(void* p){
    int mark = 1;   //开始了竞争资源!
    // int i = *(int*) p;
    int i = (int) p;
    for(int j=2; j<i/2; j++){
        if(i%j == 0){
            mark = 0;
            break;
        }
        mark = 1;
        }
    if(mark){
        printf("[%d]%d is a primer\n", (int)p, i);
    sleep(5);
    }
    mysem_add(sem,1);
    // sleep(1000);
    pthread_exit(NULL);
}