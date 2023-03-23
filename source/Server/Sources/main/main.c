#include "myepoll.h"
#include "server_func.h"
#include "tcp_server.h"
#include "user.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

#define IP "192.168.202.128"
#define PORT 6666
#define EVENTS_MAX_SIZE 128

// ???
void signalhandler(int signum)
{
    if (SIGINT == signum) // ctl+C退出程序
    {
        // 销毁在线用户链表
        ulist_destroy();
        // 销毁线程池
        exit(0);
    }
    else // 其他异常捕捉打印信号值
    {
        printf("signum = %d\n", signum);
    }
}

int main(int argc, const char *argv[])
{
    // 屏蔽异常
    for (int i = 1; i < 65; i++)
    {
        signal(i, signalhandler);
    }

    // 创建服务器套接字并绑定网络地址和端口、设置监听
    int sockfd = server_create(IP, PORT);

    printf("接口绑定成功\n");

    tpool_create(10); // 创建线程池

    printf("线程池创建成功\n");

    // 创建在线用户链表
    if (-1 == ulist_create())
    {
        printf("ulist_create error");
        exit(-1);
    }

    printf("在线链表成功\n");

    myepoll_create(EVENTS_MAX_SIZE);  // 初始化epoll对象

    printf("epoll创建成功\n");

    struct epoll_event events[EVENTS_MAX_SIZE] = {0};
    //将服务器Socket的文件描述符sockfd添加到epoll监听的集合中
    myepoll_add(sockfd);

    printf("socket添加成功\n");

    int i;
    char buf[2000] = {0};
    while(1)
    {
        int num = myepoll_wait(events, EVENTS_MAX_SIZE); // 用于保存发送请求的socket数量
        for(i = 0; i <= num; i++)
        {
            if(events[i].data.fd == sockfd) // 客户端请求连接
            {
                int cfd = server_accept(sockfd);
                if(-1 == cfd)
                {
                    continue;
                }
                printf("连接成功！\n");
                myepoll_add(cfd);
            }
            else
            {
                if(events[i].events & EPOLLIN)  // 客户端发消息
                {
                    memset(buf, 0, sizeof(buf));
                    int ret = recv(events[i].data.fd, buf, sizeof(buf), 0);
                    if(-1 == ret)
                    {
                        if(errno = EINTR)
                        {
                            continue;
                        }
                        perror("main-recv");
                    }
                    else if(0 == ret)  // 用户下线
                    {
                        printf("客户端(cfd = %d)已下线！\n", events[i].data.fd);
                        ulist_delete_bycfd(events[i].data.fd);
                        myepoll_delete(events[i].data.fd);
                        close(events[i].data.fd);
                    }
                    else
                    {
                        ctl_t *ctl = (ctl_t *)buf;
                        ctl->cfd = events[i].data.fd;
                        void *temp = calloc(1, ctl->msg_size);
                        if(NULL == temp)
                        {
                            perror("main-calloc");
                            continue;
                        }
                        memcpy(temp, buf, ctl->msg_size);
                        tpool_add_task(task_routine, temp);
                    }
                }
            }
        }
    }
    return 0;
}