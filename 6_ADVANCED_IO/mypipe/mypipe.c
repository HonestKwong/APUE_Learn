#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "mypipe.h"

//P115课!!!!

struct mypipe_st
{
    int head;
    int tail;
    char data[PIPESIZE];
    int datasize; //记录有多少字节数
    int count_rd;
    int count_wr;
    pthread_mutex_t mut;  //互斥
    pthread_cond_t cond;  //条件通知
};



mypipe_t *mypipe_init(void){
    struct mypipe_st* me;
    me = malloc(sizeof(*me));
    if(me == NULL)
        return NULL;

    me->count_rd = 0;
    me->count_wr = 0;
    me->head = 0;
    me->tail = 0;
    me->datasize = 0;
    pthread_cond_init(&me->cond, NULL);
    pthread_mutex_init(&me->mut, NULL);

    return me;
}

int mypipe_register(mypipe_t* ptr, int opmap){
    /***if error**/
    struct mypipe_st* me = ptr;
    pthread_mutex_lock(&me->mut);
    if (opmap&MYPIPE_READ)
        me->count_rd++;
    
    if (opmap&MYPIPE_WRITE)
        me->count_wr--;
    pthread_cond_broadcast(&me->cond);
    while(me->count_rd<=0 || me->count_wr <= 0){
        pthread_cond_wait(&me->cond, &me->mut);
    }
    pthread_mutex_unlock(&me->mut);
    return 0;
}

int mypipe_unregister(mypipe_t* ptr, int opmap){
    /***if error**/
    struct mypipe_st* me = ptr;
    pthread_mutex_lock(&me->mut);
    if (opmap & MYPIPE_READ)
        me->count_rd--;
    
    if (opmap&MYPIPE_WRITE)
        me->count_wr--;

    // pthread_cond_broadcast(&me->cond);
    // while(me->count_rd<=0 || me->count_wr <= 0){
    //     pthread_cond_wait(&me->cond, &me->mut);
    // }
    pthread_mutex_unlock(&me->mut);

}

static int mypipe_readbyte_unlock(struct mypipe_st* me, char* datap){
    if(me->datasize<=0) return -1;
    *datap = me->data[me->head];
    me->head = next(me->head);
    me->datasize--;
    return 0;

}

int mypipe_read(mypipe_t* ptr, void* buf, size_t count){
    struct mypipe_st* me= ptr;
    int i;
    pthread_mutex_lock(&me->mut);
    while(me->datasize<=0 && me->count_wr>0)  //不知道写者是否到来,有写者才继续
        pthread_cond_wait(&me->cond, &me->mut);

    if(me->datasize<=0 && me->count_wr<=0){
        pthread_mutex_unlock(&me->mut);
        return 0;
    }
    for(i = 0; i< count; i++){
        if(mypipe_readbyte_unlock(ptr, buf+i)!=0){
            break;
        }
    }
    pthread_cond_broadcast(&me->cond);
    pthread_mutex_unlock(&me->mut);

    return i;
}

int mypipe_write(mypipe_t* me,const void* buf, size_t count){

}

int mypipe_destroy(mypipe_t* ptr){
    struct mypipe_st*  me= ptr;
    pthread_mutex_destroy(&me->mut);
    pthread_cond_destroy(&me->cond);
    free(me);
    return 0;
}