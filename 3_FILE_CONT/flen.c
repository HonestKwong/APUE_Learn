#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

static off_t flen(const char* fname){ //这里返回不能为int，应该为off_t
    struct stat statres;
    if(stat(fname,&statres)<0){
        perror("stat()");
        exit(1);
    }

    return statres.st_size;
    
}

int main(int argc, char** argv){
    if(argc<2){
        fprintf(stderr,"usage...\n");
        exit(1);
    }
    printf("%lld\n",(long long)flen(argv[1]));
    exit(0);

}