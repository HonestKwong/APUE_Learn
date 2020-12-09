#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
    if(argc < 2) {
        fprintf(stderr, "Usage...\n");
        exit(1);    
    }
    //初始化非常重要？
    char *linebuf = NULL;
    size_t linesize = 0;
    FILE* fp = NULL;

    fp = fopen(argv[1], "r");
    if(fp == NULL){
        perror("fopen()");
        exit(1);
    }

    

    while(1){
        if(getline(&linebuf, &linesize, fp) < 0){
            break;
        }
        printf("%ld\n", strlen(linebuf));
        printf("%ld\n",linesize);
    }

    fclose(fp);


    exit(0);
}