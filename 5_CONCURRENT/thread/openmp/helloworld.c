#include <stdio.h>
#include <stdlib.h>

int main(){

//有多少个核,扔出多少个线程,我的电脑支持超线程,实际上扔出了12个线程
#pragma omp parallel
{
    puts("Hello");
    puts("world");
}
    exit(0);
}