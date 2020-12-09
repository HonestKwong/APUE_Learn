#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#define TIMESTRSIZE 1024
#define FMTSTRSIZE 1024
/*
* -y :year
* -m: month
* -d: day
* -H: hour
* -M: minutes
* -S: sencond
*/
int main(int argc, char** argv){
    //李老师提醒，每一句都有可能出错
    FILE* fp = stdout;
    time_t stamp;
    struct tm* tmStruct;
    char timestr[TIMESTRSIZE];
    stamp = time(NULL);
    tmStruct = localtime(&stamp);
    char fmstr[FMTSTRSIZE];
    fmstr[0] = '\0';


    while(1){
        int c = getopt(argc, argv, "-H:MSy:md");
        if(c<0) break;
        switch (c)
        {
        case 1:
            if(fp == stdout){            
                fp = fopen(argv[optind-1], "w");  //这里必须输入字符串“w”
                if(fp == NULL){
                    perror("fopen()");
                    fp = stdout;
                }
            }
            break;
        case 'H':
            if(strcmp(optarg, "12")==0)
                strncat(fmstr, "%I(%P) ", FMTSTRSIZE);
            else if(strcmp(optarg, "24")==0)
                strncat(fmstr, "%H ", FMTSTRSIZE);
            else {
                fprintf(stderr,"H Invalid arg\n");
                _exit(1);
            }
            break;
        case 'M':
            strncat(fmstr,"%M ",FMTSTRSIZE);
            break;
        case 'S':
            strncat(fmstr,"%S ",FMTSTRSIZE);
            break;
        case 'y':
            if(strcmp(optarg, "4")==0)
                strncat(fmstr, "%Y ", FMTSTRSIZE);
            else if(strcmp(optarg, "2")==0)
                strncat(fmstr, "%y ", FMTSTRSIZE);
            else {
                fprintf(stderr,"y Invalid arg\n");
                _exit(1);
            }
            break;
        case 'm':
            strncat(fmstr,"%m ",FMTSTRSIZE);
            break;
        case 'd':
            strncat(fmstr,"%d ",FMTSTRSIZE);
            break;
        
        default:
            break;
        }
    }

        
    strftime(timestr, TIMESTRSIZE, fmstr, tmStruct);
    strncat(timestr,"\n",TIMESTRSIZE);
        // fprintf(fp, timestr);

    fputs(timestr, fp);
    if(fp!=stdout)  //不是就不关
        fclose(fp);
    exit(0);


    
}