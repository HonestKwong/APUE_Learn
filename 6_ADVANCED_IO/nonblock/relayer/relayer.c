#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "relayer.h"
#include <pthread.h>
#include <string.h>

#define TTY1 "/dev/tty11"
#define TTY2 "/dev/tty12"
#define BUFSIZE 1024


enum{
    STATE_R = 1,
    STATE_W ,
    STATE_Ex ,
    STATE_T 
};

struct rel_fsm_st{
    int state;
    int sfd;
    int dfd;
    char buf[BUFSIZE];
    int len;
    int pos;
    char* errstr;
    long long count;
};

struct rel_job_st
{
    int job_state;
    int fd1;
    int fd2;
    struct rel_fsm_st fsm12, fsm21;
    int fd1_save, fd2_save;
    // struct timerval state, end;
};


static struct rel_job_st* rel_job[REL_JOBMAX];
static pthread_mutex_t mut_rel_job = PTHREAD_MUTEX_INITIALIZER;
static pthread_once_t init_once = PTHREAD_ONCE_INIT;

static void fsm_driver(struct rel_fsm_st* fsm){
    int ret;
    switch(fsm->state){
        case STATE_R:
        fsm->len = read(fsm->sfd, fsm->buf, BUFSIZE);
        if(fsm->len == 0) fsm->state = STATE_T;
        else if(fsm->len < 0){
            if(errno == EAGAIN)
                fsm->state = STATE_R;
            else {
                fsm->errstr = "read() err";
                fsm->state = STATE_Ex;
            }
        }
        else{
            fsm->pos = 0;
            fsm->state = STATE_W;
        }

            break;

        case STATE_W:
            ret = write(fsm->dfd,fsm->buf+fsm->pos, fsm->len);
            if(ret<0){
                if(errno == EAGAIN)
                    fsm->state = STATE_W;
                else{
                    fsm->errstr = "write() err";
                    fsm->state = STATE_Ex;
                }
            }
            else{
                fsm->pos += ret;
                fsm->len -= ret;
                if(fsm->len == 0)
                    fsm->state = STATE_R;
                else {
                    fsm->state = STATE_W;                    
                }
                    
            }
            break;

        case STATE_Ex:
            perror(fsm->errstr);
            fsm->state = STATE_T;
            break;

        case STATE_T:
            /****do sth ***/
            break;
        default:
            /****do sth ***/
            abort();
            break;
    }
}

static void* thr_relayer(void* p){
    while(1){
        pthread_mutex_lock(&mut_rel_job);
        for(int i=0; i<REL_JOBMAX;i++){
            if(rel_job[i]!=NULL){
                if(rel_job[i]->job_state == STATE_RUNNING){
                    fsm_driver(&rel_job[i]->fsm12);
                    fsm_driver(&rel_job[i]->fsm21);
                    if(rel_job[i]->fsm12.state == STATE_T && \
                        rel_job[i]->fsm21.state == STATE_T){
                            rel_job[i]->job_state == STATE_OVER;
                        }

                }
            }
        }
        pthread_mutex_unlock(&mut_rel_job);
    }

}

static void module_load(void){
    int err;
    pthread_t tid_layer;
    err = pthread_create(&tid_layer,NULL,thr_relayer,NULL);
    if(err){
        fprintf(stderr, "pthread_create():%s\n",strerror(err));
        exit(1);
    }
}
static int get_free_pos_unlock(){
    for(int i=0; i<REL_JOBMAX; i++){
        if(rel_job[i] == NULL) return i;
    }
    return -ENOSPC;
}

int rel_addjob(int fd1 ,int fd2){
    struct rel_job_st* me;
    pthread_once(&init_once,module_load);
    int pos;
    me = malloc(sizeof(*me));
    if(me == NULL)
        return -ENOMEM;
    me->fd1 = fd1;
    me->fd2 = fd2;
    me->job_state = STATE_RUNNING;
    me->fd1_save = fcntl(me->fd1, F_GETFL); 
    fcntl(me->fd1, F_SETFL, me->fd1_save|O_NONBLOCK);
    me->fd2_save = fcntl(me->fd2_save, F_GETFL); 
    fcntl(me->fd2, F_SETFL, me->fd2_save|O_NONBLOCK);   
    
    me->fsm12.sfd = me->fd1;
    me->fsm12.dfd = me->fd2;
    me->fsm12.state = STATE_R;

    me->fsm21.sfd = me->fd2;
    me->fsm21.dfd = me->fd1;
    me->fsm21.state = STATE_R;

    pthread_mutex_lock(&mut_rel_job);
    pos = get_free_pos_unlock();
    if(pos< 0){
        fcntl(me->fd1, F_SETFL, me->fd1_save);
        fcntl(me->fd2, F_SETFL, me->fd1_save);
        free(me);
        pthread_mutex_unlock(&mut_rel_job);
        return -ENOSPC;
    }

    rel_job[pos] = me;
    pthread_mutex_unlock(&mut_rel_job);

    
}

int rel_canceljob(int id){

}

int rel_waitjob(int id, struct rel_state_st* p){

}

int rel_statjob(int id, struct rel_state_st* p){

}
