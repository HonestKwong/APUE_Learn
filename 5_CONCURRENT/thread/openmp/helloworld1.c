#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
int main(){

//有多少个核,扔出多少个线程,我的电脑支持超线程,实际上扔出了12个线程
#pragma omp parallel sections
{
    #pragma omp section
    printf("[%d]Hello\n", omp_get_thread_num());
    #pragma omp section
    printf("[%d]world\n", omp_get_thread_num());
}
    exit(0);
}