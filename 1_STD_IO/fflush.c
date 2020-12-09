#include <stdio.h>
#include <stdlib.h>

int main(){
    int i;
    printf("Before while()");
    fflush(stdout);
    while(1);
    printf("After while()");
    fflush();
    exit(0);
}