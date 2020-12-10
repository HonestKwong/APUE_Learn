# APUE 6 高级IO 

*非阻塞IO  -- 阻塞IO

## *补充:有线状态机编程(非常重要)





## 1.非阻塞IO

​	简单流程: 自然流程是结构化的

​	复杂流程: 自然流程不是结构化的 

​	自然流程:作为人类最直观的流程

(补充了一个linux工具dia)

//IO密集型任务,绝大时间都在空闲!

## 2.IO多路转接

​	文件描述符的监视	

​	select(); //移植非常好, 非常古老

​	poll();	//中立 自己维护一个数组 pollfd

​	epoll();   //linux的方言,考虑移植性的时候没法用它 , 封装了一层,蒙在了内核中





```C
   NAME
       select, pselect, FD_CLR, FD_ISSET, FD_SET, FD_ZERO - synchronous
       I/O multiplexing

SYNOPSIS
       /* According to POSIX.1-2001, POSIX.1-2008 */
       #include <sys/select.h>
   /* According to earlier standards */


   int select(int nfds, fd_set *readfds, fd_set *writefds,
              fd_set *exceptfds, struct timeval *timeout);
	/* args:
	*  nfds监视的文件描述符中最大的一个+1
	*  readfds 文件描述符的读集
	*  exceptfds 异常集合
	*  write 文件描述符的写集
	*  timeout 超时设置
	*/
   void FD_CLR(int fd, fd_set *set);   //删除,从指定集合中删除fd
   int  FD_ISSET(int fd, fd_set *set);  //查看是否存在
   void FD_SET(int fd, fd_set *set); //放入fd_set
   void FD_ZERO(fd_set *set);		//清空

   #include <sys/select.h>

   int pselect(int nfds, fd_set *readfds, fd_set *writefds,
               fd_set *exceptfds, const struct timespec *timeout,
               const sigset_t *sigmask);
```




```C
NAME
       poll, ppoll - wait for some event on a file descriptor

SYNOPSIS
       #include <poll.h>
   int poll(struct pollfd *fds, nfds_t nfds, int timeout);

   #define _GNU_SOURCE         /* See feature_test_macros(7) */
   #include <signal.h>
   #include <poll.h>

   int ppoll(struct pollfd *fds, nfds_t nfds,
           const struct timespec *tmo_p, const sigset_t *sigmask);
DESCRIPTION
       poll()  performs  a similar task to select(2): it waits for one of a set
       of file descriptors to become ready to perform I/O.
       The set of file descriptors to be monitored is specified in the fds  ar‐
   gument, which is an array of structures of the following form:

       struct pollfd {
           int   fd;         /* file descriptor */
           short events;     /* requested events */
           short revents;    /* returned events */
       };
```



```C

NAME
       epoll - I/O event notification facility

SYNOPSIS
       #include <sys/epoll.h>
*  epoll_create(2)  creates  a new epoll instance and returns a file de‐
      scriptor referring to that instance.   (The  more  recent  epoll_cre‐
      ate1(2) extends the functionality of epoll_create(2).)

   *  Interest  in  particular  file  descriptors  is  then  registered via
      epoll_ctl(2), which adds items to the interest list of the epoll  in‐
      stance.

   *  epoll_wait(2) waits for I/O events, blocking the calling thread if no
      events are currently available.  (This system call can be thought  of
      as fetching items from the ready list of the epoll instance.)
    
    
```


```C
  NAME
       epoll_create, epoll_create1 - open an epoll file descriptor

SYNOPSIS
       #include <sys/epoll.h>
   int epoll_create(int size);
   int epoll_create1(int flags);
 
 DESCRIPTION
       epoll_create()  creates  a  new  epoll(7) instance.  Since Linux
       2.6.8, the size argument is ignored, but must  be  greater  than
       zero; see NOTES below.
```





     NAME
           epoll_ctl - control interface for an epoll file descriptor
    
    SYNOPSIS
           #include <sys/epoll.h>
       int  epoll_ctl(int  epfd,  int  op,  int  fd, struct epoll_event
       *event);
## 3.其他读写函数

​		readv();

​		writrev();



```C
NAME
       readv,  writev,  preadv,  pwritev,  preadv2, pwritev2 - read or
       write data into multiple buffers

SYNOPSIS
       #include <sys/uio.h>
       
   ssize_t readv(int fd, const struct iovec *iov, int iovcnt);

   ssize_t writev(int fd, const struct iovec *iov, int iovcnt);

   ssize_t preadv(int fd, const struct iovec *iov, int iovcnt,
                  off_t offset);

   ssize_t pwritev(int fd, const struct iovec *iov, int iovcnt,
                   off_t offset);

   ssize_t preadv2(int fd, const struct iovec *iov, int iovcnt,
                   off_t offset, int flags);

   ssize_t pwritev2(int fd, const struct iovec *iov, int iovcnt,
                    off_t offset, int flags);
```


​		



## 4.存储映射IO

​		mmap()

​		munmap()

```C
   
  MMAP(2)                Linux Programmer's Manual               MMAP(2)

NAME
       mmap, munmap - map or unmap files or devices into memory

SYNOPSIS
       #include <sys/mman.h>
   void *mmap(void *addr, size_t length, int prot, int flags,
              int fd, off_t offset);
   int munmap(void *addr, size_t length);

   See NOTES for information on feature test macro requirements.
```
```C
	RETURN VALUE
   On  success,  mmap()  returns a pointer to the mapped area.  On
   error, the value MAP_FAILED (that is, (void *) -1) is returned,
   and errno is set to indicate the cause of the error.

   On success, munmap() returns 0.  On failure, it returns -1, and
   errno is set to indicate the cause of the  error  (probably  to
   EINVAL).
```
stat();

fstat()



用作共享内存:shm.c





## 5.文件锁

意外解锁

fcntl();  //比较复杂一点,关家级别

lockf();

flock();

```C
NAME
       lockf - apply, test or remove a POSIX lock on an open file

SYNOPSIS
       #include <unistd.h>
       int lockf(int fd, int cmd, off_t len);
```



