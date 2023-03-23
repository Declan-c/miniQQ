#include "myepoll.h"

int epfd;

void myepoll_create(int size)
{
    //创建epoll对象
    epfd = epoll_create(size);
    if(-1 == epfd)
    {
        ERRLOG("epoll_create");
    }
}

void myepoll_add(int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;

    if(-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev))
    {
        ERRLOG("epoll_add");
        return;
    }
}

//阻塞等待事件发生
int myepoll_wait(struct epoll_event *events, int size)
{
    int num = epoll_wait(epfd, events, size, -1);
    if(-1 == num)
    {
        ERRLOG("epoll_wait");
        return -1;
    }
    return num;
}

void myepoll_delete(int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    if(0 != epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev))
    {
        perror("myepoll_delete epoll_ctl error");
        return;
    }
}

void mypoll_destroy()
{
    //关闭epoll对象的文件描述符
    if(-1 == close(epfd))
    {
        ERRLOG("close");
    }
}