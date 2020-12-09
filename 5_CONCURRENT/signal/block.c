#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static void int_handler(int s){
    write(1,"!",1);
}
int main(){

    sigset_t set;
    sigset_t set_old;
    sigset_t set_saved;
    
    signal(SIGINT, int_handler);
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    // sigprocmask(SIG_BLOCK, &set, NULL);
    sigprocmask(SIG_UNBLOCK, &set, &set_saved);
    for(int j =0 ; j<1000; j++){
    sigprocmask(SIG_BLOCK, &set, &set_old);
        for(int i=0; i<10; i++){
                write(1,"*", 1);
                sleep(1);
            }
        write(1,"\n", 1);
        sigprocmask(SIG_SETMASK, &set_old, NULL);
        // sigprocmask(SIG_UNBLOCK, &set, NULL);

    }
    sigprocmask(SIG_SETMASK, &set_saved, NULL);
    

    exit(0);
}