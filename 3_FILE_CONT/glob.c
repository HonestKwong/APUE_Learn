#include <stdio.h>
#include <stdlib.h>
#include <glob.h>

#define PAT "/etc/.*"
#if 0
int errfunc_(const char* errpath, int errno){
    puts(errpath);
    fprintf(stderr,"ERROR MSG:%s\n", strerror(errno));
    return 0;
}
#endif
int main(){
    glob_t globRes;
    int err = 0;
    err = glob(PAT,0, NULL,&globRes);
    if(err){
        printf("Error code = %d\n",err);
        exit(1);
    }

    for(int i=0; i<globRes.gl_pathc; i++)
        puts(globRes.gl_pathv[i]);
    
    globfree(&globRes);
    exit(0);
}