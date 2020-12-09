#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){

    printf("argc = %d\n", argc);

    for(int i=0; i<argc; i++)
    {
        printf("%s\n",argv[i]);
    }
    exit(0);
}