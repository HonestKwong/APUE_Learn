#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


void* func(void* p){
//    sleep(100);
    sleep(100);
   pthread_exit(NULL);
}
int main(){


    long long i = 0;
    pthread_t tid;
    pthread_attr_t attr;
    size_t size;
    int err;
    
    //现在的内核好像没有这个限制了
    //《linux内核设计与实现》上说为了与老版本的unix和linux兼容，
    //pid的最大值默认是32767（short int的最大值）
    //我的好像把pid号用完了........
    pthread_attr_init(&attr);
    pthread_attr_getstacksize(&attr,&size);
    printf("%lld\n", size);
    pthread_attr_setstacksize(&attr,1024);
    pthread_attr_getstacksize(&attr,&size);
    printf("%lld\n", size);
    for(; ;i++){
        // err = pthread_create(&tid, NULL,func,NULL); //为NULL的时候我的电脑开启的进程是22000个左右
        err = pthread_create(&tid, &attr,func,NULL); 
        if(err){
            fprintf(stderr,"pthread_create:%s\n",strerror(err));
            break;   // pthread_attr_getstacksize(&attr,&size);
    // printf("%lld\n", size);
        }
    }
    printf("counts:%lld\n",i);
    pthread_join(tid, NULL);
    pthread_attr_destroy(&attr);
    exit(0);
}