#ifndef ANYTIMER_H__
#define ANYTIMER_H__

typedef void at_jobfunc_t(void*);

static int pos;
// static struct XXX *job;

int at_addjob(int sec, at_jobfunc_t* jobp, void* arg);
/*
* return >=0 成功，返回指定任务ID
*        == -EINVAL 失败，参数非法
*        == -ENOSPC 失败，数组满
         == -ENOMEM 失败，内存空间不足
**/

int at_addjob_repeat(int , at_jobfunc_t*, void*);
/*
* return >=0 成功，返回指定任务ID
*        == -EINVAL 失败，参数非法
*        == -ENOSPC 失败，数组满
         == -ENOMEM 失败，内存空间不足
**/

int at_canceljob(int id);
/*
* return ==0 成功，指定任务已取消
*        == -EINVAL 失败，参数非法
*        == -EBUSY 失败，flag状态完成
*        == -EBUSY 失败，指定任务重复取消
**/

int at_waitjob(int id);
/*
* return ==0 成功，指定任务已成功释放
*        == -EINVAL 失败，参数非法
*        == -EBUSY 失败，指定任务为周期任务
**/

at_pausejob();  //暂停
at_resumejob(); //继续

#endif