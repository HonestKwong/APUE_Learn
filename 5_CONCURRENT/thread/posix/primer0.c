#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>


#define LEFT 30000000
#define RIGHT 30000200
#define THRNUM (RIGHT-LEFT+1)


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
        printf("%d is a primer\n", i);
    }
    // sleep(1000);
    pthread_exit(NULL);
}
int main(){
    int i, err;
    pthread_t tid[THRNUM];
    for(i = LEFT;i<=RIGHT; i++){
        // err = pthread_create(tid+(i-LEFT), NULL, thr_prime, &i); //这种会造成竞争!
        err = pthread_create(tid+(i-LEFT), NULL, thr_prime, (void*)i); //最垃圾的方式
        if(err){
            fprintf(stderr, "pthread_create failed %s", strerror(err));
            exit(1);
        }
    }
    for(i = LEFT;i<=RIGHT; i++)
        pthread_join(tid[i-LEFT],NULL);
    exit(0);

}