#ifndef _TPOOL_H_
#define _TPOOL_H_
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>

#define ERRLOG(errmsg)                                          \
    do                                                          \
    {                                                           \
        perror(errmsg);                                         \
        printf("%s - %s - %d\n", __FILE__, __func__, __LINE__); \
    } while (0)

// 任务节点结构体
typedef struct tpool_task{
    void *(*routine)(void *); // 任务函数
    void *arg;                // 任务上下文
    struct tpool_task *next;
} tpool_task_t;
// 线程池结构体
typedef struct tpool{
    int max_thr_num;   // 线程池最大线程数量
    pthread_t *thr_id; // 线程id数组首地址
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    tpool_task_t *que_head; // 任务队列头指针
    tpool_task_t *que_tail; // 任务队列尾指针
    int close;              // 标记线程池关闭
} tpool_t;

//创建线程池
int tpool_create(int max_thr_num);
// 向线程池中添加任务
int tpool_add_task(void *(*routine)(void *), void *arg);
//销毁线程池
void tpool_destroy();

#endif