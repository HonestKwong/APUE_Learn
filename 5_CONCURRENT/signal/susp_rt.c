#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define MYRTSIG (SIGRTMIN+6)

static void MYSIG_handler(int s){
    write(1,"!",1);
}
int main(){

    sigset_t set;
    sigset_t set_old;
    sigset_t set_saved;
    
    signal(MYRTSIG, MYSIG_handler);
    sigemptyset(&set);
    sigaddset(&set, MYRTSIG);
    // sigprocmask(SIG_BLOCK, &set, &set_old);
    sigprocmask(SIG_UNBLOCK, &set, &set_saved);
    sigprocmask(SIG_BLOCK, &set, &set_old);
    for(int j =0 ; j<1000; j++){
    
        for(int i=0; i<5; i++){
                write(1,"*", 1);
                sleep(1);
            }
        write(1,"\n", 1);
        sigsuspend(&set_old);
        // sigset_t tmpset;
        // sigprocmask(SIG_SETMASK, &set_old, &tmpset);
        // // sigprocmask(SIG_UNBLOCK, &set, tmpset);
        // pause();
        // sigprocmask(SIG_SETMASK,&tmpset,NULL);

    }
    sigprocmask(SIG_SETMASK, &set_saved, NULL);
    

    exit(0);
}