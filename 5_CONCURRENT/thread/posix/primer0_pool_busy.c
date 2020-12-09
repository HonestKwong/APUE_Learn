#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

//*************忙在查询法查询任务!!!!!!!!!////////////////

static int num = 0;
static pthread_mutex_t mut_num = PTHREAD_MUTEX_INITIALIZER;


#define LEFT 30000000
#define RIGHT 30000200
#define THRNUM 4


static void* thr_prime(void* p){
    int mark = 1,i;   //开始了竞争资源!
    // int i = *(int*) p;
    while(1){
        pthread_mutex_lock(&mut_num);
        while(num==0){
            pthread_mutex_unlock(&mut_num);
            sched_yield();
            pthread_mutex_lock(&mut_num);
        }
        if(num == -1){
            pthread_mutex_unlock(&mut_num);
            break; //必须解锁再退出!!!!!
            //临界区的任何一个跳转到临界区外的语句,都需要解锁再跳转!!!!
        }
        i = num;
        num = 0;
        pthread_mutex_unlock(&mut_num);
        
        for(int j=2; j<i/2; j++){
            if(i%j == 0){
                mark = 0;
                break;
            }
            mark = 1;
            }
        if(mark){
            printf("[%d]%d is a primer\n", (int)p,i);
        }
    }
    // sleep(1000);
    pthread_exit(NULL);
}
int main(){
    int i, err;
    pthread_t tid[THRNUM];
    for(i = 0;i<=THRNUM; i++){
        // err = pthread_create(tid+(i-LEFT), NULL, thr_prime, &i); //这种会造成竞争!
        err = pthread_create(tid+i, NULL, thr_prime, (void*)i); //最垃圾的方式
        if(err){
            fprintf(stderr, "pthread_create failed %s", strerror(err));
            exit(1);
        }
    }

    for(i = LEFT; i<=RIGHT;i++){
        pthread_mutex_lock(&mut_num);
        while(num != 0){
            pthread_mutex_unlock(&mut_num);
            sched_yield(); //出让调度器
            pthread_mutex_lock(&mut_num);
        }
        num = i;
        pthread_mutex_unlock(&mut_num);
    }
    pthread_mutex_lock(&mut_num);
    while(num!=0){
      pthread_mutex_unlock(&mut_num); 
      sched_yield();
      pthread_mutex_lock(&mut_num);
    }
    num = -1;
    pthread_mutex_unlock(&mut_num);
    


    for(i = 0;i<=THRNUM; i++)
        pthread_join(tid[i],NULL);
    pthread_mutex_destroy(&mut_num);
    exit(0);

}