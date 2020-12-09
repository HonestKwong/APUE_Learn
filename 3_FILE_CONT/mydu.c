#include <stdio.h>
#include <stdlib.h>
#include <glob.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define PATHSIZE 1024
static int path_noloop(const char* path){
    char* pos;
    pos = strrchr(path,'/');
    if(pos == NULL) exit(1);
    if(strcmp(pos+1,".") == 0||strcmp(pos+1,"..") == 0)
        return 1;
    return 0;
}

static int64_t mydu(const char*path){
    // if(path == NULL||path) return;
    //没有横跨递归点的变量可以放在静态区！！
    struct stat statres;
    char nextpath[PATHSIZE];
    glob_t globRes;
    int64_t sum = 0;

    if(lstat(path, &statres)<0){
        perror("lstat()");
        exit(1);
    }
    if(!S_ISDIR(statres.st_mode)){
        return statres.st_blocks;
    }

    strncpy(nextpath,path, PATHSIZE);
    strncat(nextpath,"/*", PATHSIZE);
    glob(nextpath, 0, NULL, &globRes);
    //if else 写校验，一定要些校验

    strncpy(nextpath,path,PATHSIZE);
    strncat(nextpath,"/.*", PATHSIZE);
    glob(nextpath, GLOB_APPEND , NULL, &globRes);

    
    for(int i=0; i<globRes.gl_pathc; i++){
        if(path_noloop(globRes.gl_pathv[i])) continue;
        sum+=mydu(globRes.gl_pathv[i]);
    }

    sum += statres.st_blocks; //
    globfree(&globRes);  //这个是必须要
    return sum;





    
    // glob(path, 0, NULL, )
}
int main(int argc, char** argv){
    if(argc < 2){
        fprintf(stderr,"Usage...\n");
        exit(1);
    }
    printf("%lld\n",mydu(argv[1])/2);

    exit(0);
}