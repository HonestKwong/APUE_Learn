# APUE 第八章...  进程

进程基本支持

已经进入多进程阶段

## 1.进程标识符pid

​		类型pid_t

​		命令ps

​		进程号是顺次向下使用的，不是像文件描述符选最小的

```c++
   These functions are always successful.

   #include <sys/types.h>
   #include <unistd.h>

   pid_t getpid(void);
   pid_t getppid(void);
```
## 2.进程的产生 

#### 2.1 fork();  //fflush的重要性

注意理解关键字： duplicating,意味着拷贝，克隆，一模一样等含义。

fork后父子进程的区别：fork的返回值不一样，pid不同， ppid也不同。

未决信号和文件缩不继承，资源利用量清零。

​	init进程：1号 所有进程的祖先进程

​	调度器的调度策略来决定哪个进程先运行



```c
   #include <sys/types.h>
   #include <unistd.h>

   pid_t fork(void);
//fork()  creates  a  new  process  by  “duplicating”复制  the  calling
       //process.  The new process is referred to as the child  process.
       //The calling process is referred to as the parent process.

```
time ./primer0   //记时

./primer1 |wc -l 计数

killall primer1 终止进程

ps的状态代码

PROCESS STATE CODES
       Here are the different values that the s, stat and state output
       specifiers (header "STAT" or "S") will display to describe the
       state of a process:

               D    uninterruptible sleep (usually IO)
               I    Idle kernel thread
               R    running or runnable (on run queue)
               S    interruptible sleep (waiting for an event to
                    complete)
               T    stopped by job control signal
               t    stopped by debugger during the tracing
               W    paging (not valid since the 2.6.xx kernel)
               X    dead (should never be seen)
               Z    defunct ("zombie") process, terminated but not
                    reaped by its parent
Z 僵尸进程需要收尸！ 僵尸进程不会占用太多硬件资源，但是它会占用很宝贵（非常宝贵）的资源-PID号！

### 2.2vfork(); 淘汰？

vfork和fork一样都创建一个子进程，但是它并不将父进程的地址空间完全复制到子进程中，因为子进程会立即调用exec或exit

vfork() either modifies any data other than a  variable of type pid_t used to store the return value from vfork(),or returns from the function in which vfork()  was  called,



**当前fork使用了写时复制技术，见APUE P182!  共享数据， 只读， 谁改谁复制！由此淘汰了vfork！**



## 3.进程的消亡及释放资源

wait()

waitpid()

waitid()

wait3()

wait4()

       pid_t wait(int *wstatus);
    
       pid_t waitpid(pid_t pid, int *wstatus, int options);
    
       int  waitid(idtype_t idtype, id_t id, siginfo_t *infop, int op‐
       tions);
                       /* This is the glibc and POSIX interface; see
                          NOTES for  information  on  the  raw  system
       call. */
进程分配：交叉分配、块分配 primer3.c

## 4.exec函数族

execl, execlp, execle, execv, execvp, execvpe - execute a file执行一个文件

The exec() family of functions “replaces”替换！	 the current process image  with a new process image.

**注意 fflush刷新缓冲！!!!!!!!!!!**

例子ex.c



可执行文件在磁盘上的命令是外部命令

strtok, strsep

```C
   #include <unistd.h>

   extern char **environ;   //环境变量！

   int execl(const char *pathname, const char *arg, ...
                   /* (char  *) NULL */);
   int execlp(const char *file, const char *arg, ...
                   /* (char  *) NULL */);
   int execle(const char *pathname, const char *arg, ...
                   /*, (char *) NULL, char *const envp[] */);
   int execv(const char *pathname, char *const argv[]);
   int execvp(const char *file, char *const argv[]);
   int execvpe(const char *file, char *const argv[],
                   char *const envp[]);
```
## 5.用户权限及组权限（u+s,g+s）

​		u+s //切换成

​		g+s 

*process里面有三个user id,刚开始真是一个令人感觉头痛的地方。因为很容易混淆。但是慢慢的，抓住了一些关键的原因和特点，就可以比较容易理解了。process的三个ID分别是*real user id\*\**effective user id saved set-user-id 。对于group有相同的情况，我们可以照下文类推。\**

  首先要注意的是real user id是真正process执行起来的用户user id，这个是比较清楚的，这个进程是哪个用户调用的，或者是哪个父进程发起的，这个都是很明显的。通常这个是不更改的，也不需要更改。

  然后要注意的是，当判定一个进程是否对于某个文件有权限的时候，要验证的ID是effective user id，而非real user id。那effective user id是如何被设置进来的呢？



   有u+s权限意味着：他的身份会切换成她的user？

​	有g+s权限意味着：他的身份会切换成她的user？

-rwsr-xr-x 

getuid()//

geteuid()//有效的

getgid()()

getegid()

setuid()

setgid()

setreuid()  //交换r和e 原子化交换

setregid()  //交换r和e 原子化交换



chown root mysu //切换所有者  9分40s开始解释

chmod u+s mysu //切换用户权限



## 6.观摩课：解释器文件

​		说白了就是脚本文件

​		看到脚本文件就会装载解释器！

见t.exec !#装载解释器

​		





## 7.system();

system： 理解：fork+exec+wait的封装！





## 8.进程会计

acct()

process accounting file

```C
    #include <sys/acct.h>

struct acct_v3 {
               char      ac_flag;      /* Flags */
               char      ac_version;   /* Always set to ACCT_VERSION (3)
 */
               u_int16_t ac_tty;       /* Controlling terminal */
               u_int32_t ac_exitcode;  /* Process termination status */
               u_int32_t ac_uid;       /* Real user ID */
               u_int32_t ac_gid;       /* Real group ID */
               u_int32_t ac_pid;       /* Process ID */
               u_int32_t ac_ppid;      /* Parent process ID */
               u_int32_t ac_btime;     /* Process creation time */
               float     ac_etime;     /* Elapsed time */
               comp_t    ac_utime;     /* User CPU time */
               comp_t    ac_stime;     /* System time */
               comp_t    ac_mem;       /* Average memory usage (kB) */
               comp_t    ac_io;        /* Characters transferred (unused) */
               comp_t    ac_rw;        /* Blocks read or written
                                          (unused) */
               comp_t    ac_minflt;    /* Minor page faults */
               comp_t    ac_majflt;    /* Major page faults */
               comp_t    ac_swaps;     /* Number of swaps (unused) */
               char      ac_comm[ACCT_COMM]; /* Command name */
           };
```



## 9.进程时间

```C
NAME
       times - get process times

SYNOPSIS
       #include <sys/times.h>

       clock_t times(struct tms *buf);

DESCRIPTION
       times() stores the current process times in the struct tms that
       buf points to.  The struct tms is as defined in <sys/times.h>:

           struct tms {
               clock_t tms_utime;  /* user time */
               clock_t tms_stime;  /* system time */
               clock_t tms_cutime; /* user time of children */
               clock_t tms_cstime; /* system time of children */
           };


```

```C

```

## 10.守护进程

会话，session，标识sid

终端：实际上我们拿到的是虚拟终端

守护进程不需要收尸，因为守护进程需要运行很长时间，父进程程应当退出！

setsid

setpgid

getpgid

getpgrp

有的守护进程只能开一个的时候，需要完成：

单实例守护进程：锁文件 

cd /var/run/name.pid 下面 把进程编号pid保存在里面！

启动脚本文件： etc/rc*.... 我没找到.....



       #include <sys/types.h>
       #include <unistd.h>
    
       pid_t setsid(void);
       
       setsid() creates a new session if the calling process is not a process group leader.  The calling process is the leader of the new session (i.e., its session ID is made the same as its process ID).  The calling process also becomes the process group  leader  of  a  new
           process group in the session (i.e., its process group ID is made the same as its process ID).
           The calling process will be the only process in the new process group and in the new session.
    
           Initially,  the  new  session  has  no controlling terminal.  For details of how a session acquires a controlling terminal, see creden‐
           tials(7).
    
```C
   setpgid, getpgid, setpgrp, getpgrp - set/get process group
   SYNOPSIS
       #include <sys/types.h>
       #include <unistd.h>
          int setpgid(pid_t pid, pid_t pgid);
   pid_t getpgid(pid_t pid);

   pid_t getpgrp(void);                 /* POSIX.1 version */
   pid_t getpgrp(pid_t pid);            /* BSD version */

   int setpgrp(void);                   /* System V version */
   int setpgrp(pid_t pid, pid_t pgid);  /* BSD version */
```



## 11.系统日志

与10非常相关

syslogd

如何修改何种信息写入系统日志?/etc/sys/???

写守护进程一定要会写系统日志！

```C
#include <syslog.h>

       void openlog(const char *ident, int option, int facility);  //facility 来源
       void syslog(int priority, const char *format, ...);
//priority 级别 format 类似printf
       void closelog(void);
```

