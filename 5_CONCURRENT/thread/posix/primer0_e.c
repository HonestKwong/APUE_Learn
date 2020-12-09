#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>


#define LEFT 30000000
#define RIGHT 30000200
#define THRNUM (RIGHT-LEFT+1)

struct thr_arg_st{
    int n;
};


static void* thr_prime(void* p){
    int mark = 1;   //开始了竞争资源!
    int i = ((struct thr_arg_st*)p)->n;
    // free(p); //最好在同一个模块中free
    // int i = (int) p;
    for(int j=2; j<i/2; j++){
        if(i%j == 0){
            mark = 0;
            break;
        }
        mark = 1;
        }
    if(mark){
        printf("%d is a primer\n", i);
    }
    // sleep(1000);
    // pthread_exit(NULL);
    pthread_exit(p);
}
int main(){
    void* ptr;
    int i, err;
    struct thr_arg_st *p;
    pthread_t tid[THRNUM];
    for(i = LEFT;i<=RIGHT; i++){
        p = malloc(sizeof(*p));
        if(p == NULL){
            perror("malloc");
            exit(1);
        }
        p->n = i;
        // err = pthread_create(tid+(i-LEFT), NULL, thr_prime, &i); //这种会造成竞争!
        err = pthread_create(tid+(i-LEFT), NULL, thr_prime, p); //最垃圾的方式
        if(err){
            fprintf(stderr, "pthread_create failed %s", strerror(err));
            exit(1);
        }
    }
    for(i = LEFT;i<=RIGHT; i++){  
        pthread_join(tid[i-LEFT],&ptr);
        free(ptr);  //这样malloc和free在同一个模块中去了
        }
    exit(0);

}