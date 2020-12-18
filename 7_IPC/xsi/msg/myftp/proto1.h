#ifndef PROTO_H__
#define PROTO_H__
#define KEYPATH "/etc/services"
#define KEYPROH  'a'

#define PATHMAX 1024
#define DATAMAX 1024

enum{
    MSG_PATH = 1,
    MSG_DATA,
    MSG_EOT
};

typedef struct msg_path_st{
    long mtype;              //must be MSG PATH
    char path[PATHMAX]; //ASCIIZ带伪0的串
} msg_path_t;

typedef struct msg_data_st
{
    long mtype;             //must be MSG PATH
    char data[DATAMAX];        
    int datalen;
}msg_data_t;

typedef struct msg_eot_st
{
    long mtype;             //must be MSG PATH
}msg_eot_t;

//用共用体来进行接收!
union msg_s2c_un{
    long mtype;  //用它来区分来的是哪一种的结构体类型
    msg_data_t datamsg;
    msg_eot_t eotmsg;
};

#endif