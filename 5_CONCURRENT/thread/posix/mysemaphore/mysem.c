#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "mysem.h"

struct  mysem_st
{
    int value;
    pthread_mutex_t mut;
    pthread_cond_t cond;
};

mysem_t* mysem_init(int initval){
    struct mysem_st* me;    //将void* 和 mysem_st* 注意区分!!!!,不然不知道分配多少个字节!
    // me = (mysem_st*)malloc(sizeof(struct  mysem_st));
    me = malloc(sizeof(*me));
    printf("&me:%d",(int)me);
    if(me == NULL)
        return NULL;
    me->value = initval;
    pthread_cond_init(&me->cond, NULL);
    pthread_mutex_init(&me->mut, NULL);
    return me;
    
}

int mysem_add(mysem_t* ptr,int n){
    struct mysem_st* me = ptr;
    pthread_mutex_lock(&me->mut);

    me->value += n;
    pthread_cond_broadcast(&me->cond);
    pthread_mutex_unlock(&me->mut);
    return n;
}

int mysem_sub(mysem_t* ptr,int n){
    struct mysem_st* me = ptr;
    pthread_mutex_lock(&me->mut);
    while(me->value<n){
        pthread_cond_wait(&me->cond, &me->mut);
    }
    me->value -= n;
    pthread_mutex_unlock(&me->mut);
    return n;
}

int mysem_destroy(mysem_t* ptr){
    struct mysem_st* me = ptr;
    pthread_mutex_destroy(&me->mut);
    pthread_cond_destroy(&me->cond);
    free(me);
    return 0;
}
