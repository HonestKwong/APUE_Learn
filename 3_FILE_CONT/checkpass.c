#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <shadow.h>
#include <string.h>
#include <crypt.h>

//这个程序用makefile失败，用gcc成功不知道为什么！
//gcc -o checkpass checkpass.c -lcrypt(成功)
int main(int argc, char** argv){
    char* input_pass = NULL;  //输入的字符串，去除回车
    char* crypted_pass = NULL;  //加密的密码？
    struct spwd* shadowline;    //保存在文件里的
    if(argc<2){
        fprintf(stderr,"Usage...\n");
        exit(1);
    }
    input_pass = getpass("Password:");

    shadowline = getspnam(argv[1]);

    crypted_pass = crypt(input_pass, shadowline->sp_pwdp);
    if(strcmp(shadowline->sp_pwdp, crypted_pass) == 0)
        puts("OK");
    else
        puts("fail!");  
    exit(0);
}