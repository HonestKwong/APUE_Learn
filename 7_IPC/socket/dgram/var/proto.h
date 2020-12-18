#ifndef PROTO_H__
#define PROTO_H__

#define RCVPORT "1989"

#define NAMEMAX (512-8-8)  //512是UDP的推荐长度,第一个8是UDP抱头,第二个8是定长的字节数
struct  msg_st
{
    
    uint32_t math;
    uint32_t chinese;
    uint8_t name[1]; //变长结构体的实现
}__attribute__((packed));










#endif