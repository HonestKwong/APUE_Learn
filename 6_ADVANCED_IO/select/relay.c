#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>

#define TTY1 "/dev/tty11"
#define TTY2 "/dev/tty12"
#define BUFSIZE 1024

enum{
    STATE_R = 1,
    STATE_W ,
    STATE_AUTO, //做了一条线
    STATE_Ex ,
    STATE_T 
};

struct fsm_st{
    int state;
    int sfd;
    int dfd;
    char buf[BUFSIZE];
    int len;
    int pos;
    char* errstr;
};

static void fsm_driver(struct fsm_st* fsm){
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

static int max(int a, int b){
    if(a>b) return a;
    return b;

}

static void relay(int fd1, int fd2){
    int fd1_save;
    int fd2_save;
    struct fsm_st fsm12, fsm21;
    fd_set rset, wset;
    fd1_save = fcntl(fd1, F_GETFL);
    fcntl(fd1, F_SETFL, fd1_save| O_NONBLOCK); //因为read是默认阻塞读!
    fd2_save = fcntl(fd2, F_GETFL);
    fcntl(fd2, F_SETFL, fd2_save| O_NONBLOCK);

    fsm12.state = STATE_R;
    fsm12.sfd = fd1;
    fsm12.dfd = fd2;

    fsm21.state = STATE_R;
    fsm21.sfd = fd2;
    fsm21.dfd = fd1;

    while(fsm12.state != STATE_T || fsm21.state != STATE_T){
        //布置监视任务
        FD_ZERO(&rset);
        FD_ZERO(&wset);
        if(fsm12.state == STATE_R)
            FD_SET(fsm12.sfd, &rset);
        if(fsm12.state == STATE_W)
            FD_SET(fsm12.dfd, &wset);  
        if(fsm21.state == STATE_R)
            FD_SET(fsm21.sfd, &rset);
        if(fsm21.state == STATE_W)
            FD_SET(fsm21.dfd, &wset);              
        if(fsm12.state<STATE_AUTO || fsm21.state<STATE_AUTO)
        {
            if(select(max(fd1,fd2)+1, &rset, &wset,NULL,NULL)<0){ 
                //监视现场和监视结果放在同一个空间,需要重新设置监视线程,大循环!
                //这个函数本身有缺陷
                //监视的事件太过单一了,可以算作异常的情况太多
                if(errno == EINTR) continue;
                perror("select()");
                exit(1);
            }
        }

        //监视
        

        //查看监视结果


        //根据监视结果有条件的推动
        if(FD_ISSET(fsm12.sfd, &rset)|| FD_ISSET(fsm12.dfd, &wset) || fsm12.state>STATE_AUTO)
        fsm_driver(&fsm12);
        if(FD_ISSET(fsm21.sfd, &rset)|| FD_ISSET(fsm21.dfd, &wset) || fsm21.state>STATE_AUTO)
        fsm_driver(&fsm21);
    }
    
    fcntl(fd1, F_SETFL, fd1_save);
    fcntl(fd2, F_SETFL, fd2_save);
}

int main(){
    int fd1,fd2;
    fd1 = open(TTY1, O_RDWR);
    if(fd1<0){
        perror("open()");
        exit(1);
    }
    write(fd1, "TTY1\n",5);
    fd2 = open(TTY2, O_RDWR);
    if(fd2<0){
        perror("open()");
        exit(1);
    }
    write(fd2, "TTY2\n",5);

    relay(fd1,fd2);

    close(fd1);
    close(fd2);

    exit(0);
}