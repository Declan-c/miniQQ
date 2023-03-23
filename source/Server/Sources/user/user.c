#include "user.h"

static ulist_t *ul = NULL;

// 创建在线用户链表
int ulist_create()
{
    ul = (ulist_t *)malloc(sizeof(ulist_t));
    if (NULL == ul)
    {
        perror("ulist_create:malloc error");
        return -1;
    }
    ul->count = 0;
    ul->head = (user_t *)calloc(1, sizeof(user_t));
    if (NULL == ul->head)
    {
        perror("ulist_create:calloc error");
        return -1;
    }
    ul->head->next = NULL;
    return 0;
}

// 头插法插入用户
int ulist_insert(int cfd, const char *user_id, const char *nickname)
{
    printf("user: %d | %s | %s\n", cfd, user_id, nickname);
    // 初始化一个在线用户
    user_t *on_user = (user_t *)malloc(sizeof(user_t));
    if (NULL == on_user)
    {
        perror("ulist_insert:malloc");
        return -1;
    }
    on_user->cfd = cfd;
    strcpy(on_user->id, user_id);
    strcpy(on_user->nickname, nickname);
    // on_user->ban = ban;
    // on_user->vip = vip;
    on_user->next = NULL;
    // 头插
    on_user->next = ul->head->next;
    ul->head->next = on_user;
    ul->count++;
    return 0;
}

// 通过用户id查找
user_t *ulist_search_byid(char *id)
{
    user_t *p = ul->head->next;
    while (p)
    {
        if (0 == strcmp(id, p->id))
        {
            return p;
        }
        p = p->next;
    }
    return NULL;
}
// 通过客户端socket文件描述符查找
user_t *ulist_search_bycfd(int cfd)
{
    user_t *p = ul->head->next;
    while (p)
    {
        if (cfd == p->cfd)
        {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

// 通过cfd删除一个在线用户
int ulist_delete_bycfd(int cfd)
{
    user_t *p = ul->head;
    while (p->next)
    {
        if (cfd == p->next->cfd)
        {
            user_t *temp = p->next;
            p->next = temp->next;
            temp->next = NULL;
            free(temp);
            temp = NULL;
            ul->count--;
            return 0;
        }
        p = p->next;
    }
    return -1;
}

// 获得在线链表
ulist_t *get_ulist()
{
    return ul;
}

// 销毁在线用户链表
void ulist_destroy()
{
    user_t *p = ul->head;
    user_t *temp = NULL;
    while (p)
    {
        temp = p;
        p = p->next;
        free(temp);
        temp = NULL;
    }
    free(ul);
    ul = NULL;
}