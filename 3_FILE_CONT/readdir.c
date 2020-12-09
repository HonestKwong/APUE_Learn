#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>

#define PAT "/etc"

int main(){
    DIR *dp = NULL;
    dp = opendir(PAT);
    struct dirent *cur =NULL;
    if(dp == NULL){
        perror("opendir()");
        exit(1);
    }
    while((cur = readdir(dp))!=NULL)
        puts(cur->d_name);
    closedir(dp);
    exit(0);
}