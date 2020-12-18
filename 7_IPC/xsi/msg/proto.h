#ifndef PROTO_H__
#define PROTO_H__

#define KEYPATH "/etc/services"
#define KEYPROJ 'g'
#define NAMESIZE 32

struct  msg_st
{
    long mtype;    //为什么一定要有!?
    char name[NAMESIZE];
    int math;
    int chinese;
};


#endif