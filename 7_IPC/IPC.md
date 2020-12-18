# APUE 7 进程间通信 IPC

## 1 管道PIPE

​	内核提供: 单工,  自同步机制

​	匿名管道:  文件系统无法看到

​	命名管道:  文件系统可以看到



NAME
       pipe, pipe2 - create pipe

SYNOPSIS
       #include <unistd.h>

```C
   /* On Alpha, IA-64, MIPS, SuperH, and SPARC/SPARC64; see NOTES */
   struct fd_pair {
       long fd[2];
   };
   struct fd_pair pipe();

   /* On all other architectures */
   int pipe(int pipefd[2]);   //创建的匿名管道!

   #define _GNU_SOURCE             /* See feature_test_macros(7) */
   #include <fcntl.h>              /* Obtain O_* constant definitions */
   #include <unistd.h>

   int pipe2(int pipefd[2], int flags);
```


有一个shell指令叫做mkfifo创建命名管道!

两个管道实现双工

cat fifo

date>fifo  必须有读有写才会结束!

```C
NAME
       mkfifo, mkfifoat - make a FIFO special file (a named pipe)

SYNOPSIS
       #include <sys/types.h>
       #include <sys/stat.h>
	int mkfifo(const char *pathname, mode_t mode);   //命名管道

   #include <fcntl.h>           /* Definition of AT_* constants */
   #include <sys/stat.h>

   int mkfifoat(int dirfd, const char *pathname, mode_t mode);
```
## 2  XSI  -> SysV

ipcs shell指令

IPC -> Inter-Process Communication

主动端:先发包的一方

被动端:先收包的一方(先运行)





P ulimit -a 显示消息中缓存的最大的  POSIX message queues     (bytes, -q) 819200

ipcrm -Q  0x670100d4  -> man ipcrm

没有亲缘关系进程间的通信



key:ftok();

Message Queues

msgget();

msgop();   //oprations

msgctl;



Semaphore Arrays

semget();

when semflg is zero and key does not have the value IPC_PRIVATE

semop();

semctl();



Shared Memory

shmget();

shmop();

shmctl();



IPC比较套路123



3 网络套接字socket



讨论: 跨主机的传输需要注意的问题

1.字节序问题:

**大端存储: 低地址处放高字节**

**小端存储: 高地址处放高字节**

主机字节序:host

解决:网络字节序:network

_ to_ _: htons, htobl, ntohs, ntohl

2.对齐

​	struct{

​		int i;

​		float f;

​		char ch; 

​	}

解决方法: 不对齐



3.类型长度的问题:

int 

char

解决:int32_t, uint32_t, int64_t, int8_t,uint8_t



SOCKET是什么:  简单的理解成中间层 

IO是一切实现的基础,UNIX一切皆文件

如果成功,传输的是

       NAME
           socket - create an endpoint for communication
    
    SYNOPSIS
           #include <sys/types.h>          /* See NOTES */
           #include <sys/socket.h>
       int socket(int domain, int type, int protocol);	 //第一个参数,协议族
       													//第二个参数,实现
       													//3 协议



```C
SOCK_STREAM     Provides sequenced, reliable, two-way,  connection-based
 byte  streams.   An out-of-band data transmission mechanism may be supported. //可靠的,双工的
SOCK_DGRAM      Supports datagrams (connectionless, unreliable  messages
                   of a fixed maximum length).

   SOCK_SEQPACKET  Provides a sequenced, reliable, two-way connection-based
                   data transmission path for datagrams  of  fixed  maximum
                   length;  a consumer is required to read an entire packet
                   with each input system call.

   SOCK_RAW        Provides raw network protocol access.

   SOCK_RDM        Provides a reliable datagram layer that does not guaran‐
                   tee ordering.

   SOCK_PACKET     Obsolete  and  should  not  be used in new programs; see
                   packet(7).  //很少使用
```




```C
NAME
       inet_pton - convert IPv4 and IPv6 addresses from text to binary form

SYNOPSIS
       #include <arpa/inet.h>   
   int inet_pton(int af, const char *src, void *dst);
```



    NAME
           inet_ntop - convert IPv4 and IPv6 addresses from binary to text form
    
    SYNOPSIS
           #include <arpa/inet.h>
    const char *inet_ntop(int af, const void *src,
                             char *dst, socklen_t size);


报式套接字:

被动端(先运行)

1.取得SOCKET

2.给SOCKET取得地址 (绑定本地的端口)

3.收/发消息

4.关闭SOCKET



主动端

1.取得SOCKET

2.给SOCKET取得地址 (可省略)

3.发/收消息

4.关闭SOCKET

__attribute__((packed)) //告诉编译器不要对齐!

用到的函数,见var/ ./proto.h  rcver.c snder.c

Socket();

bind();

sendto();

rcvfrom();

inet_pton();

inet_ntop();

setsockopt();

getsocketopt();



多点通讯: 广播(全网广播,子网广播), 多播/组播 (多播更灵活,多播可以选择是否加入多播组,使用起来特别灵活) 224.0.0.1

man 7 socket   socket option 选择实现属性

man 7 ip

man 7 udp

man 7 tcp

setsockopt(sd,  SOL_SOCKET, SO_BROADCAST)

ip ro sh 查看默认路由

ip ro add default via 192.168.1.123



ip ad sh



ifconfig



if_nametoindex()

UDP : 丢包 ->由阻塞造成->流控->停等式流控



流式套接字:

TTL TIME TO LIVE //可以默认经过的路由的个数  unix一般是64?  windows 128

停等时间RTT

TCP的半连接状态, 半连接池.半连接洪水,非常下流的攻击形式,只进行两次握手,但不进行第三次握手.这个很难,攻击半连接池.

解决方法,压根不再要半连接池,(对端ip+对端port + 我端ip+我端port+proto协议+salt(内核产生!))进行hash得到cookie

如何区分A,B,C,D四类网络地址,他们分别有什么作用

流式套接字实现方式,和代码实现步骤:



C端(主动端):

1.获取socket

2.给socket取得地址(可省的)

3.发送连接

4.收发消息

5.关闭连接



S端:

1.获取socket

2.给socket取得地址

3.将socket置为监听模式

4.接受连接

5.收/发消息

6.关闭



nc 127.0.0.1 1997

telnet 127.0.0.1 1997

了解APACHE  

httpd start

/var/www/html/jpg

eog打开图片



epoll事件边缘触发?





