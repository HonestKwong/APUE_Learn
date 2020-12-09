#include <stdio.h>
#include <stdlib.h>

extern char** environ;

int main(){
    int i;
    for(i=0; environ[i]!=NULL; i++){
        puts(environ[i]);
    }
    exit(0);

}