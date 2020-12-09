#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define TIMESTRSIZE 1024
//计算100天后
int main(){
    //李老师提醒，每一句都有可能出错
    time_t stamp;
    struct tm* tmStruct;
    char timestr[TIMESTRSIZE];
    stamp = time(NULL);
    tmStruct = localtime(&stamp);
    strftime(timestr, TIMESTRSIZE, "Now:%Y-%m-%d",tmStruct);
    puts(timestr);

    
    tmStruct->tm_mday += 100;
    (void)mktime(tmStruct);  //为什么void.利用mltime修改！
    strftime(timestr, TIMESTRSIZE, "100Days Later:%Y-%m-%d",tmStruct);
    puts(timestr);

    exit(0);


    
}