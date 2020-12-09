# APUE  并发（信号、线程）

## 一、信号(第十章)

同步

异步

异步事件的处理：查询法（稠密的异步事件），通知法（稀疏的异步事件）

信号处理函数尽量小,小到一闪而过最好.

**信号处理函数中最好不要用标准IO可能刷新共同的缓冲区,最好使用系统调用!!**

### 1.信号的概念

​	信号是软件中断

​	信号的响应依赖于中断，

### 2.signal()

段错误 修改 ulimit -c 保存出错线程最大值 ， ulimit -c 10240 会生成core.1234

```c
   #include <signal.h>

   typedef void (*sighandler_t)(int);

   sighandler_t signal(int signum, sighandler_t handler);  //handler 处理程序
	void(*signal(int signum, void (*func)(int)))(int);
```
**信号会打断阻塞的系统调用！**  例如star.c 中的sleep()



### 3.信号的不可靠(行为不可靠)

标准信号会丢失不是说明信号不可靠， 信号行为不可靠，系统第一次调用没结束的时候可能发生了第二次调用！

### 4.可重入函数

​	所有的系统调用都是可重入的，一部分库函数也是可重入的。



_r 可重入版本， 

```C
  #include <time.h>

   char *asctime(const struct tm *tm);
   char *asctime_r(const struct tm *tm, char *buf);

   char *ctime(const time_t *timep);
   char *ctime_r(const time_t *timep, char *buf);

   struct tm *gmtime(const time_t *timep);
   struct tm *gmtime_r(const time_t *timep, struct tm *result);

   struct tm *localtime(const time_t *timep);
   struct tm *localtime_r(const time_t *timep, struct tm *result); //多一个自己给定的参数，空间有自己分配，结果不会被信号终端干扰
```
​	

#include <string.h>

       void *memcpy(void *dest, const void *src, size_t n);

DESCRIPTION
       The  memcpy()  function  copies n bytes from memory area src to
       memory area dest.  The memory areas must not overlap.  Use mem‐
       move(3) if the memory areas do overlap.

### 5.信号的响应过程

其实进程和线程不分家，进程是人定的容器，我们现在的编程是单进程单线程。

信号从收到到响应有一个不可避免的延迟，中断不打断，看不到信号，**从kernel返回用户态** mask & pending

思考：信号如何忽略掉的？ 将mask某一位置0

​			标准信号为什么要丢失？ 用位图，来一万个相同信号，最终都是置一个1

​			标准信号的响应没有严格的顺序！先响应比较严重的信号

​			不能从信号处理 函数中**随意**的往外跳！ 不能从setjmp跳出来（但是各家协定不一样），sigsetjmp，siglongjmp 会保存mask？

### 6.常用函数

​	kill();

​	//由于多半的信号是终止，这个函数是发信号

```C
       #include <sys/types.h>
       #include <signal.h>

       int kill(pid_t pid, int sig);

```

​	raise();

```C
		#include <signal.h>

       int raise(int sig);
      //The  raise()  function sends a signal to the calling process or
     //thread.  In a single-threaded program it is equivalent to

           
		kill(getpid(), sig);

       //In a multithreaded program it is equivalent to

       pthread_kill(pthread_self(), sig);


```

​	alarm();

```C
   #include <unistd.h>

   unsigned int alarm(unsigned int seconds);
       //alarm()  arranges  for  a SIGALRM signal to be delivered to the
       //calling process in seconds seconds.

```
gcc -S XXXX.c 产生汇编文件

-O1 产生优化！ 

**volatile   //不要轻信内存中的值？ 去数值真正的空间取数值！**	



例：使用单一计时器构造一组函数，实现任意数量的计时器或者 	记得回去写anytimer.c!!

alarm 或 setitimer

​	setitimer();

```C
get or set value of an interval timer   
SYNOPSIS
       #include <sys/time.h>
   int getitimer(int which, struct itimerval *curr_value);
   int setitimer(int which, const struct itimerval *new_value,
                 struct itimerval *old_value);
```
​	pause();

​    abort(); //终止，并会保存一个core dump的出错现场！



    NAME
           abort - cause abnormal process termination
    
    SYNOPSIS
           #include <stdlib.h>
       void abort(void);

​	system();

```C
 During  execution  of the command, SIGCHLD will be blocked, and
   SIGINT and SIGQUIT will be ignored, in the process  that  calls
   system(). //在运行过程中阻塞一个信号，忽略两个信号！
```


​	sleep(); -> alarm() + pause(); 源码里有sleep就很棒槌,要么测试忘删了

​	nanosleep(); 可以用于替换sleep

​	usleep();

​	select(); 超时设置，只传timeout，可以成为休眠！

​	

​	**static volatile sig_atomic_t token = 0; //加上原子操作保证++和--是一气呵成的**





令牌桶 三个要素： CPS、BURST、TOKEN

永远不要觉得自己在写一个完整的函数，永远不要觉得自己在单枪匹马的工作！

### 7.信号集

信号集类型sigset_t

       #include <signal.h>
    
       int sigemptyset(sigset_t *set);
    
       int sigfillset(sigset_t *set);
    
       int sigaddset(sigset_t *set, int signum);
    
       int sigdelset(sigset_t *set, int signum);
    
       int sigismember(const sigset_t *set, int signum);


### 8.信号屏蔽字/pending集处理

sigprocmask();

```C
   #include <signal.h>

   /* Prototype for the glibc wrapper function */
   int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
```
sigpending();  //响应完的信号，说明信号来没来

```C
   #include <signal.h>

   int sigpending(sigset_t *set);
```


### 9.扩展

​	sigsuspend(); 见susp

​	显示的作用是等待一个信号，和pause很像！

       NAME
           sigsuspend, rt_sigsuspend - wait for a signal
    
    SYNOPSIS
           #include <signal.h>
       int sigsuspend(const sigset_t *mask);



​	**sigaction();** 非常重要！！

```C
       struct sigaction {
           void     (*sa_handler)(int);
           void     (*sa_sigaction)(int, siginfo_t *, void *); //这个三参数的形式
           sigset_t   sa_mask;
           int        sa_flags;
           void     (*sa_restorer)(啊void);
       };
```


```C
  The siginfo_t data type  is  a  structure  with  the  following
   fields:

       siginfo_t {
           int      si_signo;     /* Signal number */
           int      si_errno;     /* An errno value */
           int      si_code;      /* Signal code */  ！！！这个很要用！！
           int      si_trapno;    /* Trap number that caused
                                     hardware-generated signal
                                     (unused on most architectures) 
                                     ////////////////...........
```
见mydeamon的用法！



while true; do kill -ALRM 219711; done  **//mytbf.c 没有识别信号来自哪里！**



getcontext();

setcontext();





### 10.实时信号

实时信号需要排队

实时信号保存在哪个文件里?

susp_rt.c

**实时信号不丢失!!!!!**





## 二、线程

### 1.线程的概念

一个正在运行的函数

创建出来的线程都是兄弟

用到的库应该默认支持多线程并发!

posix线程是一套标准,而不是实现

openmp线程标准

线程标识:pthread_t 可能是整形,可能是结构体

ps axm

ps -a -L

信号和多线程小范围混用合适



pthread();

SYNOPSIS
       #include <pthread.h>

​	**Compile and link with -pthread.**

       int pthread_equal(pthread_t t1, pthread_t t2);
    
       


pthread_self();

NAME
       pthread_self - obtain ID of the calling thread

SYNOPSIS
       #include <pthread.h>

   **Compile and link with -pthread.**

       pthread_t pthread_self(void);
    
    






### 2.线程的创建

NAME
       pthread_create - create a new thread

SYNOPSIS
       #include <pthread.h>

```C
   int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                      void *(*start_routine) (void *), void *arg);

   Compile and link with -pthread.
```

1.thread 回填一个线程标识

2 .attr指定属性 默认情况属性 **可以解决80%的问题**

 3.void *(*start_routine) (void *) 这个是兄弟线程的函数 void* * 可以传各类地址



On success, pthread_create() returns 0; on error, it returns an **error number**, and the contents of *thread are undefined.

只能用strnumber来区分错误信号

线程的调度取决于调度器策略,一开始create1 打印不出线程中的内容?  线程还没来的及调度,进程就结束了exit(0),所以打印不出线程中的内容! 我自己加上pause();就可以看到了.

























###   线程的终止

3种方式: 1.线程从启动例程中返回,返回就是线程的退出码

				2. 线程可以被同一进程中的其他线程取消  //算是异常终止的一种方式
   				3. 线程调用pthread_exit()函数,相当于进程阶段的exit



pthread_exit()

NAME
       pthread_exit - terminate calling thread

SYNOPSIS
       #include <pthread.h>

       void pthread_exit(void *retval);
    
       Compile and link with -pthread.


进程的收尸

NAME
       pthread_join - join with a terminated thread

SYNOPSIS
       #include <pthread.h>

```C
   int pthread_join(pthread_t thread, void **retval); //

   Compile and link with -pthread.
```
**retval  可以给它一个一级指针的地址  

RETURN VALUE
       On success, pthread_join() returns 0; on error, it  returns  an
       error number



  线程栈的清理

pthread_cleanup_push(); //挂钩子函数 

pthread_cleanup_pop()  //取钩子函数



NAME
       pthread_cleanup_push, pthread_cleanup_pop - push and pop thread
       cancellation clean-up handlers

SYNOPSIS
       #include <pthread.h>

       void pthread_cleanup_push(void (*routine)(void *),
                                 void *arg);
       void pthread_cleanup_pop(int execute);
    
       Compile and link with -pthread.
​	上述是个宏, 可以用gcc cleanup.c -E 来查看

有多少个push,就有多少个pop



### 线程的取消

​	一定会用到线程的取消

​	pthread_cancel

NAME
       pthread_cancel - send a cancellation request to a thread

SYNOPSIS
       #include <pthread.h>

```C
   int pthread_cancel(pthread_t thread);

   Compile and link with -pthread.
```
取消有两种状态:允许和不允许.

如果设置允许取消又分为:异步取消cancel和推迟取消cancel(默认) ->推迟至cancel点再响应

cancel点:POSIX定义的cancel点,都是可能引发阻塞的系统调用

NAME
       pthread_setcancelstate, pthread_setcanceltype - set cancelability state and type

SYNOPSIS
       #include <pthread.h>

```C
   int pthread_setcancelstate(int state, int *oldstate); //设置是否允许取消
   int pthread_setcanceltype(int type, int *oldtype);  //设置取消方式

   Compile and link with -pthread.
```


NAME
       pthread_testcancel  - request delivery of any pending cancellation request

SYNOPSIS

```C
   #include <pthread.h>
   void pthread_testcancel(void); //什么都不做,就是一个取消点
```



​		线程分离

​			pthread_detach()



NAME
       pthread_detach - detach a thread

SYNOPSIS


```C++
   #include <pthread.h>   //已经被detach的线程不能用join回收
   int pthread_detach(pthread_t thread);
```

静态的代码是共用的,

线程的栈是独立的

栈的

stack size              (kbytes, -s) 8192  //可以开多少个线程



### 3.线程同步

​		互斥量

pthread_mutex_t

pthread_mutex_init

pthread_mutex_destroy ??? 我的man手册上没有?

pthread_mutex_lock

pthread_mutex_trylock

pthread_mutex_unlock

NAME
       pthread_mutex_init — destroy and initialize a mutex 这个是

SYNOPSIS
       #include <pthread.h>

```C
       int pthread_mutex_destroy(pthread_mutex_t *mutex);
       int pthread_mutex_init(pthread_mutex_t *restrict mutex,
           const pthread_mutexattr_t *restrict attr);
       pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

```
NAME
       pthread_mutex_lock, pthread_mutex_trylock, pthread_mutex_unlock
       — lock and unlock a mutex

SYNOPSIS
       #include <pthread.h>

```C
   int pthread_mutex_lock(pthread_mutex_t *mutex);    //阻塞 死等
   int pthread_mutex_trylock(pthread_mutex_t *mutex);  //非阻塞
   int pthread_mutex_unlock(pthread_mutex_t *mutex);
```
阻塞调用是指调用结果返回之前，当前线程会被挂起。调用线程只有在得到结果之后才会返回。
非阻塞调用指在不能立刻得到结果之前，该调用不会阻塞当前线程。

**临界区的任何一个跳转到临界区外的语句,都需要解锁再跳转!!!!**

**临界区中的函数跳转也是一个问题,如果函数跳转回不来会产生死锁!!!**

sched_yield() 出让调度器



 pthread_once(); 实现某个模块的单次初始化只被调用一次



```C
NAME
       pthread_once — dynamic package initialization

SYNOPSIS
       #include <pthread.h>
   int pthread_once(pthread_once_t *once_control,
       void (*init_routine)(void));
   pthread_once_t once_control = PTHREAD_ONCE_INIT;
```


条件变量:

pthread_cond_t

pthread_cond_init();

pthread_cond_destroy();



NAME
       pthread_cond_destroy, pthread_cond_init — destroy and  initialize condition variables

SYNOPSIS
       #include <pthread.h>

```C
   int pthread_cond_destroy(pthread_cond_t *cond);
   int pthread_cond_init(pthread_cond_t *restrict cond,
       const pthread_condattr_t *restrict attr);
   pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
```
NAME
       pthread_cond_broadcast, pthread_cond_signal — broadcast or sig‐
       nal a condition

SYNOPSIS
       #include <pthread.h>

```C
   int pthread_cond_broadcast(pthread_cond_t *cond); //叫醒所有等待
   int pthread_cond_signal(pthread_cond_t *cond);    //随机叫醒一个等待,只能唤醒一个
```




NAME
       pthread_cond_timedwait, pthread_cond_wait — wait on a condition

SYNOPSIS
       #include <pthread.h>

```C
   int pthread_cond_timedwait(pthread_cond_t *restrict cond,
       pthread_mutex_t *restrict mutex,
       const struct timespec *restrict abstime);       
   int pthread_cond_wait(pthread_cond_t *restrict cond,
       pthread_mutex_t *restrict mutex);
```
​    **pthread_cond_broadcast(&me->cond);**

​    **pthread_mutex_unlock(&me->mut);  可以先通知再解锁!**



信号量: 哲学家问题

读写锁: 听起来有点像吸烟者问题hhhh  可能出现写者饿死

​	读锁相当于共享机制

​	写锁相当于互斥机制

​	

### 4.线程相关属性

pthread_attr_init	

NAME
       pthread_attr_init,  pthread_attr_destroy  -  initialize and de‐
       stroy thread attributes object



```C
SYNOPSIS
   #include <pthread.h>   
   int pthread_attr_init(pthread_attr_t *attr);
   int pthread_attr_destroy(pthread_attr_t *attr);
```
pthread_attr_setdetachstate  

pthread_attr_setstack

pthread_attr_setstackaddr

pthread_attr_setstacksize

见man手册

线程同步的属性

​	互斥量属性:

NAME
       pthread_mutexattr_destroy, pthread_mutexattr_init — destroy and
       initialize the mutex attributes object

SYNOPSIS
       #include <pthread.h>

```C
   int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
   int pthread_mutexattr_init(pthread_mutexattr_t *attr);
```


NAME
       pthread_mutexattr_getpshared,   pthread_mutexattr_setpshared  -
       get/set process-shared mutex attribute

SYNOPSIS
       #include <pthread.h>

       int pthread_mutexattr_getpshared(const pthread_mutexattr_t *attr,
                                        int *pshared);
       int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr,
                                        int pshared);


 



clone

```C
SYNOPSIS
       /* Prototype for the glibc wrapper function */
	#define _GNU_SOURCE
    #include <sched.h>

   int clone(int (*fn)(void *), void *stack, int flags, void *arg, ...
                 /* pid_t *parent_tid, void *tls, pid_t *child_tid */ );
      /* For the prototype of the raw clone() system call, see NOTES */

   long clone3(struct clone_args *cl_args, size_t size);
```



```C
   CLONE_FILES (since Linux 2.0)
          If CLONE_FILES is set, the calling process and the child
          process share the same file descriptor table.  Any  file
          descriptor  created  by  the  calling  process or by the
          child process is also valid in the other process.  Simi‐
          larly, if one of the processes closes a file descriptor,
          or changes its  associated  flags  (using  the  fcntl(2)
          F_SETFD  operation), the other process is also affected.
          If a process sharing a file descriptor table  calls  ex‐
          ecve(2),  its  file  descriptor table is duplicated (un‐
          shared).
```


```C
   CLONE_NEWPID (since Linux 2.6.24)
          If CLONE_NEWPID is set, then create the process in a new
          PID namespace.  If this flag is not set, then  (as  with
          fork(2))  the  process  is created in the same PID name‐
          space as the calling process.
```


```C
   CLONE_PARENT (since Linux 2.3.12)
          If CLONE_PARENT is set, then the parent of the new child
          (as returned by getppid(2)) will be the same as that  of
          the calling process.
```


pthread_mutexattr_settype();

pthread_mutexattr_gettype();

NAME
       pthread_mutexattr_settype — set the mutex type attribute

SYNOPSIS
       #include <pthread.h>

       int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);

条件变量:

```C
NAME
       pthread_condattr_init — initialize the  condition  variable  at‐
       tributes object

SYNOPSIS
       #include <pthread.h>
  int pthread_condattr_init(pthread_condattr_t *attr);
  DESCRIPTION
       Refer to pthread_condattr_destroy().
```



读写锁属性:





5.重入

   多线程中的IO.所有的IO函数都支持多线程并发,(都是加了锁的)

puts; getchar,都支持多线程,**以下是不支持多线程并发的,从名字上体现出来!**

SYNOPSIS
       #include <stdio.h>

```C
   int getc_unlocked(FILE *stream);
   int getchar_unlocked(void);
   int putc_unlocked(int c, FILE *stream);
   int putchar_unlocked(int c);

   void clearerr_unlocked(FILE *stream);
   int feof_unlocked(FILE *stream);
   int ferror_unlocked(FILE *stream);
   int fileno_unlocked(FILE *stream);
   int fflush_unlocked(FILE *stream);
   int fgetc_unlocked(FILE *stream);
   int fputc_unlocked(int c, FILE *stream);
   size_t fread_unlocked(void *ptr, size_t size, size_t n,
                         FILE *stream);
   size_t fwrite_unlocked(const void *ptr, size_t size, size_t n,
                         FILE *stream);

   char *fgets_unlocked(char *s, int n, FILE *stream);
   int fputs_unlocked(const char *s, FILE *stream);

   #include <wchar.h>

   wint_t getwc_unlocked(FILE *stream);
   wint_t getwchar_unlocked(void);
   wint_t fgetwc_unlocked(FILE *stream);
   wint_t fputwc_unlocked(wchar_t wc, FILE *stream);
   wint_t putwc_unlocked(wchar_t wc, FILE *stream);
   wint_t putwchar_unlocked(wchar_t wc);
   wchar_t *fgetws_unlocked(wchar_t *ws, int n, FILE *stream);
   int fputws_unlocked(const wchar_t *ws, FILE *stream);
```


   线程与信号的关系



```C
NAME
       pthread_sigmask - examine and change mask of blocked signals

SYNOPSIS
       #include <signal.h>
int pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset);
```





```C
NAME
       sigwait - wait for a signal

SYNOPSIS
       #include <signal.h>  
int sigwait(const sigset_t *set, int *sig);  
```



```C
NAME
       pthread_kill - send a signal to a thread

SYNOPSIS
       #include <signal.h>   
int pthread_kill(pthread_t thread, int sig);
```



 线程与fork

​	用的最广泛的是posix线程,openmp线程

​	posix :创键, 收尸,取消,退出, 互斥行为:条件, 查询,信号量

​	openmp --> www.OpenMP.org :通过编译器?? 跨语言的比较"简单"的标准?











































