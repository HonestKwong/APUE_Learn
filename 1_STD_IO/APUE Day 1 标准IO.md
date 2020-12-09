# APUE Day 1 标准I/O

## 1.杂记

### 1.1 make的使用

**make**是一个工具程式（Utility software），經由讀取叫做“makefile”的文件，[自動化建構](https://zh.wikipedia.org/wiki/自動化建構)軟體。它是一種转化文件形式的工具，转换的目标称为“target”；与此同时，它也检查文件的依赖关系，如果需要的话，它会调用一些外部软件来完成任务。它的依赖关系检查系统非常简单，主要根据依赖文件的修改时间进行判断。大多数情况下，它被用来[编译](https://zh.wikipedia.org/wiki/编译)[源代码](https://zh.wikipedia.org/wiki/源代码)，生成[结果代码](https://zh.wikipedia.org/wiki/目标代码)，然后把结果代码连接起来生成[可执行文件](https://zh.wikipedia.org/wiki/可执行文件)或者[库文件](https://zh.wikipedia.org/wiki/库文件)。

### 1.2 判断返回值数据保存在哪个区

当发现函数返回值是指针时，需要考虑数据保存在静态区，还是在堆上

栈(F)    静态区 (F)   堆(T)

```C++
FILE *fopen(const char *path, const char *mode){
        static FILE tmp;  //不能放在静态区, 不然打开不了一个
        FILE *tmp
        tmp = malloc(sizeof(FILE)) //将tmp放在堆区    
	tmp. =  ;
    .......
    return &tmp  //错误
    return tmp //直接返回堆区的
    //有逆操作的函数返回值一定在堆上
}
```





## 2.相关流

unix最多打开的流stream：1024个 使用命令ulimit -a 查看打开流的最大数量

然后默认打开流 stdin stdout 还有stderr标准输入、标准输出、和标准错误



保存文件的权限鸟叔P153,rw-rw---,对应不同用户的权限

0666& ！umask 

umask = 0002



getc原本是用宏实现的

vim

diff 比较两个文件是否相同



echo 1> /tmp/out

sprintf 来实现itoa的功能

尽量少使用scanf( %s)

ls -l flen.c 查看字节数



### MakeFile

书写makefile

空洞文件， 用fseek 将offset将内存拉长占磁盘

### 缓冲区的作用

大多数情况是好事，合并系统调用



行缓冲：换行时候刷新，满了的时候刷新，强制刷新（标准输出是这样的，因为是终端设备）



全缓冲：满了的时候刷新，强制刷新（默认，只要不是终端设备）



无缓冲：如stderr，需要立即输出内容的。



### Exit(1) 和 return 1 有什么区别？



### 临时文件 

1.如何不冲突

2.及时销毁

3.tmpnam tmpfile