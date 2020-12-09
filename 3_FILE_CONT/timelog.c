#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FNAME "/tmp/out_timelog"
#define BUFSIZE 1024

int main()
{
    time_t stamp;
    struct tm* tmStruct = NULL;
    FILE* fp = NULL;
    int count =0;
    char buf[BUFSIZE];
    fp = fopen(FNAME,"a+");
    if(fp ==NULL){
        perror("fopen()");
        exit(1);
    }
    while(fgets(buf,BUFSIZE,fp)!=NULL)
        count++;
    
    while(1){
        time(&stamp);
        tmStruct=localtime(&stamp);
        fprintf(fp,"%-4d %d-%d-%d %d:%d:%-2d\n",++count,\
        tmStruct->tm_year+1900,tmStruct->tm_mon+1,tmStruct->tm_mday,\
        tmStruct->tm_hour,tmStruct->tm_min,tmStruct->tm_sec);
        fflush(fp);
        sleep(1);  //这个报错了
    }
    // fwrite();
    fclose(fp);

    exit(0);
}