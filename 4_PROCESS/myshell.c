#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <glob.h> //利用通配符生产argv？

#define DELIMS "\t\n"

struct cmd_st{
    glob_t globres; //通配符信息
};

static void prompt(void){ //用于显示命令行的开头
    printf("mysh-0.1$");
}

static void parse(char* line, struct cmd_st* res){
    char* tok;
    // glob_t globRes; 
    int i = 0;
    while (1)
    {
        tok = strsep(&line, DELIMS); //拆分
        if(tok == NULL) break;
        if(tok[0]=='\0') continue; //就继续？
        glob(tok,GLOB_NOCHECK|GLOB_APPEND*i,NULL, &res->globres);
        i = 1; //第一次不能APPEND！
    }
    
}

int main(){
    glob_t globRes;  
    char* linebuf = NULL;
    size_t linebuf_size = 0;
    struct cmd_st cmd;
    while(1)
    {
        prompt(); //打印提示符
        if(getline(&linebuf ,&linebuf_size,stdin)<0) //获取输入
            break;  //读一行
        parse(linebuf, &cmd);
        if(0) { //如果是内部命令
        //do something
        }
        else  //如果是外部命令
        {
            pid_t pid = fork();
            if( pid<0){
                perror("fork()");
                exit(1);
            }

            if( pid==0){
                execvp(cmd.globres.gl_pathv[0],cmd.globres.gl_pathv);
                perror("execvp()");
                exit(1);
            }
            else{
                wait(NULL);
            } 
        }
    }
    exit(0);
}