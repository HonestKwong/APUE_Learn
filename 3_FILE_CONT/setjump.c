#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
    static jmp_buf save;
static void d(void){
    printf("%s():Begin.\n",__FUNCTION__);
    printf("%s():OK jump.\n",__FUNCTION__);
    longjmp(save,0);
    printf("%s():End.\n");
}

static void c(void){
    printf("%s():Begin.\n",__FUNCTION__);
    printf("%s():Call c().\n",__FUNCTION__);
    d();
    printf("%s():c() return.\n",__FUNCTION__);
    printf("%s():End.\n");
    

}

static void b(void){
    printf("%s():Begin.\n",__FUNCTION__);
    printf("%s():Call b().\n",__FUNCTION__);
    c();
    printf("%s():b() return.\n",__FUNCTION__);
    printf("%s():End.\n"); 

}
static void a(void){
    int ret;
    printf("%s():Begin.\n",__FUNCTION__);
    ret = setjmp(save);
    if(ret == 0){
    printf("%s():Call a().\n",__FUNCTION__);
    b();
    printf("%s():a() return.\n",__FUNCTION__);
    }
    else{
        printf("%s():jumped back here with code %d \n",__FUNCTION__,ret);

    }
    printf("%s():End.\n",__FUNCTION__);
    

}







int main(){
    a();
    
    exit(0);

}