#ifndef SERVER_CONF_H__
#define SERVER_CONF_H__
#define DEFAULT_MEDIADIR "/home/kwong/Music"
#define DEFAULT_IF "wlp6s0"        //这里网卡设置本机的网卡

enum{
    RUN_DAEMON = 1,
    RUN_FOREGROUND
};

struct  server_conf_st
{
    char* rcvport;
    char* mgroup;
    char* media_dir;  //媒体库所在位置
    char runmode;     //运行模式
    char* ifname;
};
extern struct server_conf_st server_conf;
extern int serversd;
extern struct sockaddr_in sndaddr;



#endif