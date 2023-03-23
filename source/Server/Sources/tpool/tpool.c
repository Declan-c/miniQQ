#include "tpool.h"

static tpool_t *tpool = NULL;

// 工作者线程函数（取任务，执行任务）
static void *thread_work(void *arg)
{
    tpool_task_t *task;
    while(1){
        pthread_mutex_lock(&(tpool->mutex));
        while(NULL == tpool->que_head && 0 == tpool->close){
            pthread_cond_wait(&(tpool->condition), &(tpool->mutex));
        }
        //查看线程池是否关闭，若已关闭，则退出线程
        if(1 == tpool->close){
            pthread_mutex_unlock(&(tpool->mutex));
            pthread_exit(NULL);
        }

        //从任务队列取出任务并执行
        task = tpool->que_head;
        tpool->que_head = tpool->que_head->next;
        task->next = NULL;
        pthread_mutex_unlock(&(tpool->mutex));
        task->routine(task->arg);
        //完成后释放
        free(task->arg);
        free(task);
        task = NULL;
    }
}

// 创建线程池
int tpool_create(int max_thr_num)
{
    if (max_thr_num <= 0)
    {
        return -1;
    }

    // 给线程池分配空间
    tpool = (tpool_t *)malloc(sizeof(tpool_t));
    if (NULL == tpool)
    {
        ERRLOG("tpool_malloc");
        return -1;
    }

    // 初始化tpool结构体成员
    tpool->max_thr_num = max_thr_num;
    tpool->close = 0; // 标记线程池打开
    tpool->thr_id = NULL;
    tpool->que_head = tpool->que_tail = NULL;
    if (pthread_mutex_init(&(tpool->mutex), NULL) != 0)
    {
        ERRLOG("pthread_mutex_init");
        return -1;
    }
    if (pthread_cond_init(&(tpool->condition), NULL) != 0)
    {
        ERRLOG("pthread_cond_init");
        return -1;
    }

    // 创建工作者线程
    tpool->thr_id = (pthread_t *)calloc(max_thr_num, sizeof(pthread_t));
    if (!tpool->thr_id)
    {
        ERRLOG("thr_id_calloc");
        exit(-1);
    }
    for (int i = 0; i < tpool->max_thr_num; i++)
    {
        if (pthread_create(&(tpool->thr_id[i]), NULL, thread_work, NULL) != 0)
        {
            ERRLOG("pthread_create");
            return -1;
        }
    }
    return 0;
}

// 向线程中添加任务
int tpool_add_task(void *(*routine)(void *), void *arg)
{
    if (routine == NULL || arg == NULL)
    {
        printf("%s:Invalid argument.", __FUNCTION__);
        return -1;
    }
    // 为任务分配空间
    tpool_task_t *task = (tpool_task_t *)malloc(sizeof(tpool_task_t));
    if (NULL == task)
    {
        ERRLOG("task_malloc");
        return -1;
    }
    // 任务结构体成员赋值
    task->routine = routine;
    task->arg = arg;
    task->next = NULL;
    // 将任务结点添加到任务队列中
    pthread_mutex_lock(&(tpool->mutex));
    if (NULL == tpool->que_head)
    {
        // 队列为空
        tpool->que_head = task;
        tpool->que_tail = task;
    }
    else
    {
        tpool->que_head->next = task;
        tpool->que_tail = task;
    }
    // 通知工作者线程有新任务
    pthread_cond_signal(&(tpool->condition));
    pthread_mutex_unlock(&(tpool->mutex));

    return 0;
}

// 销毁线程池
void tpool_destroy()
{
    tpool_task_t *unfinish;
    if(tpool->close == 1){
        return ;
    }
    tpool->close = 1;
    //唤醒所有阻塞线程
    pthread_mutex_lock(&(tpool->mutex));
    pthread_cond_broadcast(&(tpool->condition));
    pthread_mutex_unlock(&(tpool->mutex));
    //回收线程资源
    for(int i = 0; i < tpool->max_thr_num; i++){
        pthread_join(tpool->thr_id[i], NULL);
    }
    free(tpool->thr_id);
    //释放未完成的任务
    while(tpool->que_head){
        unfinish = tpool->que_head;
        tpool->que_head = tpool->que_head->next;
        free(unfinish->arg);
        free(unfinish);
    }
    //销毁互斥锁和条件变量
    pthread_mutex_destroy(&(tpool->mutex));
    pthread_cond_destroy(&(tpool->condition));
    free(tpool);

}