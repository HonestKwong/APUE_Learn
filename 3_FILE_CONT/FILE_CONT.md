# AUPE文件系统： 第四章

知识性内容 

### 类ls的实现，如myls 

touch 新建文件夹

类ls的实现， 如myls -l -a -i -n

## 一、目录和文件

### 1.获取文件属性

stat:通过文件路径获取属性，面对符号链接文件时获取的是所指向的目标文件的属性

fstat：通过文件描述符获取属性

lstat：面对符号链接文件时获取的是符号链接文件的属性

### 2.文件访问权限

st_mode 是一个16位的位图，用于表示文件

### 3.umask

0666 & ～umask 

umask 0022

作用：防止产生权限过松的文件！！

### 4.文件权限的更改/管理

chmod（change mode）

 fchmod() 这个传入的文件描述符

### 5.粘住位

t位，使用缩小范围， 原本给某一个可执行二进制命令设置当前t位，保存某一个命令的使用痕迹。现在用来给目录设置t位，比如/tmp



### 6.文件系统：FAT,UFS

​		文件系统：文件或数据的存储和管理

​		FAT16/32 静态单链表 无法随机访问，承载能力有限，  //做一个文件系统，只是个大作业

​		UFS（与FAT在同一时期）类似于一棵树，有1、2、3级文件表。UFS不适合保存小文件，大量小文件查找慢。

i位图、 i块位图、 inode(存放块地址)、 块

​		面试题：

1.函数，两个无符号的整型数，返回也是无符号的整型数，比较大小，不能用比较和判断

2.如何判断32位无符号数中0和1的数量



文件名，存在目录文件中

目录文件中存在 inode和filename  目录项就是这两个文件的合称



### 7.硬链接，符号链接

​	硬链接是目录项的同义词，且建立硬链接有限制：不能给分区建立，不能给目录建立。

符号链接优点：可跨分区，可以给目录建立。

ln bigfile.c bigfilelink.c

​	link

​	unlink

​	remove

​	rename

 

### 8.utime

可更改文件的最后读和最后改的时间

​	 change  file  last access and modification

### 9.目录的创建和销毁

mkdir Create the DIRECTORY(ies), if they do not already exist.

rmdir Remove the DIRECTORY(ies), **if they are empty.**

### 10.更改当前工作路径

cd利用chdir封装 changes the current working directory of the  calling
       process to the directory specified in path.

		fchdir s identical to chdir(); the only difference is that
	   the directory is given as an open file descriptor.
	getcwd These functions return a null-terminated string containing an
	   absolute pathname that is the current  working  directory  of
	   the  calling  process.  The pathname is returned as the func‐
	   tion result and via the argument buf, if present.
​		

chroot 假根技术：





### 11.分析目录/读取目录内容

glob:解析模式和通配符

find pathnames matching a pattern, free memory
       from glob()

flags

GLOB_APPEND



opendir()

closedir()

readdir(3)

rewinddir()

seekdir()

telldir()

GLOB_NOCHECK 没有通配符



//用上述代码一套完成mydu



mydu.c 最好考虑文件权限

好用的函数 strchr strrchr：找最右面的字符

如果一个变量单纯的在递归点递之前或之后，那可以放到静态区去存放

### dcb-lsp



## 二、系统数据文件和信息

### 2.1/etc/passwd

1.getpwmnam

2.getpwuid

         struct passwd {
               char   *pw_name;       /* username */
               char   *pw_passwd;     /* user password */
               uid_t   pw_uid;        /* user ID */
               gid_t   pw_gid;        /* group ID */
               char   *pw_gecos;      /* user information */
               char   *pw_dir;        /* home directory */
               char   *pw_shell;      /* shell program */
           };
### 2.2/etc/group

getgrgid

getgrnam



### 2.3/etc/shadow

getspname

       The getspnam() function returns a pointer to a  structure  con‐
       taining the broken-out fields of the record in the shadow pass‐
       word database that matches the username name.
struct spwd *getspnam(const char *name);



```C
       struct spwd {
           char *sp_namp;     /* Login name */
           char *sp_pwdp;     /* Encrypted password */
           long  sp_lstchg;   /* Date of last change
                                 (measured in days since
                                 1970-01-01 00:00:00 +0000 (UTC)) */
           long  sp_min;      /* Min # of days between changes */
           long  sp_max;      /* Max # of days between changes */
           long  sp_warn;     /* # of days before password expires
                                 to warn user to change it */
           long  sp_inact;    /* # of days after password expires
                                 until account is disabled */
           long  sp_expire;   /* Date when account expires
                                 (measured in days since
                                 1970-01-01 00:00:00 +0000 (UTC)) */
           unsigned long sp_flag;  /* Reserved */
       };
```


crypt 加密的函数 Crypt Library (libcrypt, -lcrypt)



getpass

this function is obsolete(过时的).  Do not use it. 2



相当重要，不能轻易更改

root 是神级用户，但不能chmod 将文件变为x为可执行

**hash 混淆** x%5 = 2 不能还原。 不可逆 如原串相同，所得串也相同1，防备管理员监守自盗

加密 - 揭秘

加密：安全：攻击成本大于收益 md5

安全？ 穷举：口令随机校验（两次重复输入）





### 2.4时间戳：

time_t   char* struct tm

time();

gmtime();

localtime();  astruct tm *localtime(const time_t *timep)

time_t mktime(struct tm *tm);  //可能改变传入

       size_t strftime(char *s, size_t max, const char *format,
                       const struct tm *tm);
         struct tm {
               int tm_sec;    /* Seconds (0-60) */
               int tm_min;    /* Minutes (0-59) */
               int tm_hour;   /* Hours (0-23) */
               int tm_mday;   /* Day of the month (1-31) */
               int tm_mon;    /* Month (0-11) */
               int tm_year;   /* Year - 1900 */
               int tm_wday;   /* Day of the week (0-6, Sunday = 0) */
               int tm_yday;   /* Day in the year (0-365, 1 Jan = 0) */
               int tm_isdst;  /* Daylight saving time */
           };




## 三、进程环境

### 1 main 函数

### 2 进程终止

正常终止： 

​			从main函数返回	return 0； 返回给父进程  //在命令行是发给shell！！ echo $? 返回上一条运行状态

​			调用exit 终止进程 库函数

​			调用_exit或 _Exit  系统调用

​			最后一个线程从其启动例程返回

​			最后一个线程调用pthread_exit



异常终止：

​			调用abort

​			接到一个信号并终止

​			最后一个线程对其取消请求作出响应

atexit(): 钩子函数， 有点像析构函数

 int atexit(void (*function)(void));

       The atexit() function registers the given function to be called
       at normal process termination, either via exit(3) or via return
       from the program's main().  Functions so registered are  called
       in  the  reverse  order of their registration; no arguments are
       passed


int func(){

​		return 0/1/2;

}

int main(){

​	int f = func();

​	........????????????/

​	switch(f){

​			case 0:

​			case 1:

​			case 2:

​			default: _exit(); //直接退出，防止故障扩大！！

}

}

### 3 命令行参数分析

getopt() //ls -a -i -n /etc /tmp .

见mydate_getopt.c



getopt_long()

### 4 环境变量

key = value

getenv

setenv //改变的情况

putenv //，描述和setenv相同，这个函数没有const修饰

### 5 C程序的存储空间布局

​		pmap(1)

### 6 库

​	动态库

​	静态库

​	手工装载库

according to the ISO C standard, casting between function pointers and 'void *', as done above, produces undefined results.

```C
   #include <dlfcn.h>

   void *dlopen(const char *filename, int flags);

   int dlclose(void *handle);

   #define _GNU_SOURCE
   #include <dlfcn.h>

   void *dlmopen (Lmid_t lmid, const char *filename, int flags);

   Link with -ldl
   
   #include <dlfcn.h>

       char *dlerror(void);
       void *dlvsym(void *handle, char *symbol, char *version);

//obtain address of a symbol in a shared object
       or executable



```
### 7 函数跳转

​			不是简单的函数调用//找树的一个节点不用一下全部跳出去

​			setjump() int setjmp(jmp_buf env); 

​			longjump() //不能轻易跳 void longjmp(jmp_buf env, int val

If the programmer mistakenly passes the value 0 in val, the "fake" return
   will instead return 1.

### 8 资源的获取及控制

int getrlimit(int)

int getrlimit(int resource, struct rlimit *rlim);
int setrlimit(int resource, const struct rlimit *rlim);

struct rlimit {
               rlim_t rlim_cur;  /* Soft limit */
               rlim_t rlim_max;  /* Hard limit (ceiling for rlim_cur) */
           };













































