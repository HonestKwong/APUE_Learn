#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define THRNUM 20
#define FNAME "/tmp/out"
#define LINESIZE 1024

static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

static void* thr_add(void* p){
    FILE *fp;
    char linebuf[LINESIZE];
    fp = fopen(FNAME, "r+");
    if(fp == NULL){
        perror("fopen() failed");
        exit(1);
    }
    
    pthread_mutex_lock(&mut);
    /****        临界区       ******/
    fgets(linebuf, LINESIZE, fp);
    fseek(fp, 0, SEEK_SET); //定位到文件首?
    fprintf(fp,"%d\n",atoi(linebuf)+1);  //老师是单核的,所以没有出现竞争
    //线程七手八脚打开用一个文件.产生覆盖写,可以用sleep(1)放大故障.
    fclose(fp);
    // sleep(1);
    /****        临界区       ******/
    pthread_mutex_unlock(&mut);
    

    pthread_exit(NULL);
}


int main(){
    pthread_t tid[THRNUM];
    int err;
    for(int i=0; i<THRNUM; i++){
        err = pthread_create(tid+i, NULL , thr_add, NULL);
        if(err){
            fprintf(stderr,"pthread_create failed %s\n", strerror(err));
            exit(0);
        }
        
    }
    for(int i=0; i<THRNUM; i++){
        pthread_join(tid[i], NULL);
    }
    pthread_mutex_destroy(&mut);




    exit(0);
}