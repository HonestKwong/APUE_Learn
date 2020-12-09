# APUE 系统调用IO

## 1.文件描述符

文件描述符fd是在文件IO中贯穿始终的类型

文件描述符的概念（整型数，数组的下标，文件描述符优先使用当前可用范围内最小的） 0 stdin 1 stdout 2stderr （有一个打开计数器！） 这些文件描述符是默认打开的

文件IO操作： open,close, read, write, lseek

### 1.1将文件IO于标准IO的区别

**举例：**传达室老大爷跑邮局 fflush() 加急

**区别：**响应速度（文件IO）&吞吐量（STDIO）

**面试：**如何使一个程序变快？ 

**提醒：**两种IO不可混用

**转换：**fileno， fdopen 得到的pos基本上不是一样的

strace用于追踪系统调用的顺序（明显看出吞吐量的区别）

### 1.2 IO的效率问题

习题：更改mycpy_sysio.c中bufsize大小对程序多大影响。注意性能最佳拐点以及出现错误的大小

time + 运行一个程序，会计算在user和sys层面所消费的时间的统计



time ./mycpy_sysio /etc/services abc
open():dfd: Permission denied

real	0m0.001s
user	0m0.000s
sys	0m0.001s

### 1.3文件共享

面试：写程序删除一个文件的第10行

补充函数：truncate/ftruncate.



### 1.4原子操作

不可分割的操作

原子：不可分割的最小单位

原子操作的作用：解决竞争和冲突

如tmpnam



程序中的重定向： dup， dup2

同步：sync， fsync， fdatasync

fcntl()：文件描述符所变的魔术几乎都来源于该函数

ioctl()：设备相关的内容

/dev/fd/目录:这是一个虚目录

kwong@kwong:~/Project/APUE/2_SYS_IO$ ls -l /dev/fd/
总用量 0
lrwx------ 1 kwong kwong 64 11月 24 21:23 0 -> /dev/pts/0
lrwx------ 1 kwong kwong 64 11月 24 21:23 1 -> /dev/pts/0
lrwx------ 1 kwong kwong 64 11月 24 21:23 2 -> /dev/pts/0
lr-x------ 1 kwong kwong 64 11月 24 21:23 3 -> /proc/54070/fd





阻塞IO和非阻塞I/O的区别在于是否通过NON BLOCK打开

重载和变参，编译的时候会不会报错

CFLAGS += -D_FILE_OFFSET_BITS = 64 -D_GNU_SOURCE -WALL

       //三参实现有Create, 两参没后create， 这不是用重载实现的，使用变参函数实现的
       int open(const char *pathname, int flags);
       int open(const char *pathname, int flags, mode_t mode);
**行缓冲，全缓冲，无缓冲 很重要！**