#include <stdio.h>
#include <stdlib.h>
#include "client.c"

/**
* -M --mgroup  指定多播组
* -P --port  指定接受端口
* -p --player  指定播放器
* -H --help  显示帮助
**/

struct client_conf_st client_conf ={\
    .rcvport = DEFAULT_RCVPORT,\
    .mgroup = DEFAULT_MGROUP,
    .player_cmd = DEFAULT_PLAYERCMD
};




int main(int argc, char** argv){
    
/* 初始化 
*   级别： 默认值< 配置文件< 环境变量< 命令行参数
*   
*
**/
    getopt_long();

    socket();



    pipe();
    
    fork();
    //子进程，调用解码器 exec

    //父进程：从网络上收包，发送给子进程


    exit(0);
}