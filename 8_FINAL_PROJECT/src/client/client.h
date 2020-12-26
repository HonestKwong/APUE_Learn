#ifndef CLIENT_H__
#define CLIENT_H__

#define DEFAULT_PLAYERCMD "/usr/bin/mpg123 - > /dev/null" //不希望输出信息就重定向,加-号标识播放标准输入上来的内容上!

#include "proto.h"
struct client_conf_st
{
    char* rcvport;  //本地内容可以使用指针
    char* mgroup;
    char* player_cmd;

};


extern struct client_conf_st client_conf;


#endif