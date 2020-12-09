#include <stdio.h>
#include <stdlib.h>

#include "mytbf.h"
#include <errno.h>
#include <signal.h>
#include <unistd.h>
static int inited = 0;
static __sighandler_t alrm_handler_save;

static struct mytbf_st* job[MYTBF_MAX];

struct mytbf_st{
    int cps;
    int token;
    int burst;
    int pos;
};

static void alrm_handler(int s){
    int i;
    alarm(1);
    for(int i=0; i<MYTBF_MAX; i++){
        if(job[i]!= NULL){
            job[i]->token += job[i]->cps;
            if(job[i]->token > job[i]->burst)
                job[i]->token = job[i]->burst;
        }
    }
}

module_unload(){
    signal(SIGALRM, alrm_handler_save);
    alarm(0);
    for(int i = 0; i<MYTBF_MAX; i++){
        free(job[i]);
    }

}

static int module_load(void){
        alrm_handler_save = signal(SIGALRM, alrm_handler);
        alarm(1);

        atexit(module_unload); //钩子函数异常退出清理一下现场！
        return 0;
}

static int get_free_pos(void){
    for(int i=0; i<MYTBF_MAX; i++){
        if(job[i]==NULL) return i;  
    }
    return -1;
}

mytbf_t* mytbf_init(int cps, int burst){
    struct mytbf_st* me;
    // me = malloc(sizeof(mytbf_t));
    if(!inited){
        // signal(SIGALRM, ...);
        // alarm(1);
        module_load();
        inited = 1;
    }

    int pos = get_free_pos();
    if(pos<0) return NULL;

    me = malloc(sizeof(*me));   //malloc的用法我还不是特别熟悉！
    if(me == NULL) return NULL;
    me->cps = cps;
    me->token = 0;
    me->burst = burst;
    me->pos = pos;
    job[pos] = me;
    
    
    return me;
}
static int min(int a, int b){
    if(a>b) return b;
    return a;
}

int mytbf_fetchtoken(mytbf_t* ptr, int size){
    struct mytbf_st *me = ptr;  //C语言的转换！
    if(size<=0) return -EINVAL;
    while(me->token <= 0)
        pause();
    int n = min(me->token, size);
    me->token -= n;
    return n;
}

int mytbf_returntoken(mytbf_t* ptr, int size){
    if(size<=0) return -EINVAL;
    struct mytbf_st *me = ptr;  //C语言的转换！
    me->token +=size;
    if(me->token > me->burst){
        me->token = me->burst;
    }
    return size;

}

int mytbf_destroy(mytbf_t* ptr){
    struct mytbf_st *me = ptr;  //C语言的转换！

    job[me->pos] = NULL;
    free(ptr);
    return 0;

}

