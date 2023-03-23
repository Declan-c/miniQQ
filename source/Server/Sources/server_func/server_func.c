#include "server_func.h"

/******客户端处理函数******/
// 注册
static void reg_handler(sql_msg_t *reg_msg)
{
    ser_msg_t ser_msg;
    ser_msg.ctl.cfd = -1; // 初始化为-1
    ser_msg.ctl.msg_size = sizeof(ser_msg);
    mysqlite_open();
    // 执行sql语句，成功则代表注册成功
    if (0 == mysqlite_exec(reg_msg->sql_buf))
    {
        // 通知客户端注册成功
        ser_msg.ctl.cmd = REGISTER;
        strcpy(ser_msg.reply, "注册成功");
    }
    else
    {
        ser_msg.ctl.cmd = OPT_ERR;
        strcpy(ser_msg.reply, "注册失败");
    }
    send_msg(reg_msg->ctl.cfd, &ser_msg, sizeof(ser_msg)); // 给客户端发送消息
}

// 登录
static void login_handler(sql_msg_t *login_msg)
{
    ser_msg_t ser_msg;
    ser_msg.ctl.cfd = -1;
    ser_msg.ctl.msg_size = sizeof(ser_msg_t);
    mysqlite_open();
    info_t user_info; // 用于保存查询到的用户信息
    char age[4] = {0};
    char *data[15] = {user_info.myid, user_info.nickname, user_info.password, user_info.security, user_info.sex, user_info.tel, age};
    if (0 == mysqlite_getdata(login_msg->sql_buf, data, ONLINE))
    {
        // 检测重复登录
        if (NULL == ulist_search_byid(user_info.myid))
        {
            user_info.age = atoi(age);
            // 将用户插入在线链表
            ulist_insert(login_msg->ctl.cfd, user_info.myid, user_info.nickname);
            ser_msg.ctl.cmd = LOGIN; // 通知客户端登录成功
            
            printf("登录成功\n");

            memcpy(ser_msg.reply, &user_info, sizeof(info_t));
        }
        else
        {
            ser_msg.ctl.cmd = OPT_ERR;
            strcpy(ser_msg.reply, "你的账号已登录，不能重复登录！");
            printf("重复登录！\n");
        }
    }
    else
    {
        ser_msg.ctl.cmd = OPT_ERR;
        strcpy(ser_msg.reply, "登录失败，账号或密码错误！");
    }
    send_msg(login_msg->ctl.cfd, &ser_msg, sizeof(ser_msg_t));
}

// 找回密码
static void find_psw_handler(sql_msg_t *find_psw_msg)
{
    mysqlite_open();
    ser_msg_t ser_msg;
    ser_msg.ctl.cfd = -1;
    ser_msg.ctl.msg_size = sizeof(ser_msg_t);
    char psw[21] = {0};
    if (0 == mysqlite_getdata(find_psw_msg->sql_buf, psw, FIND) && 0 != strlen(psw))
    {
        ser_msg.ctl.cmd = FIND_PASSWORD;
        strcpy(ser_msg.reply, psw);
    }
    else
    {
        ser_msg.ctl.cmd = OPT_ERR;
        strcpy(ser_msg.reply, "找回密码错误！");
    }
    send_msg(find_psw_msg->ctl.cfd, &ser_msg, sizeof(ser_msg_t));
}

// 聊天消息转发与保存
static void chat_handler(chat_msg_t *chat_msg)
{
    printf("chat = %s\n", chat_msg->chat_buf);
    ser_msg_t ser_msg;
    ser_msg.ctl.cfd = -1;
    ser_msg.ctl.msg_size = sizeof(ser_msg_t);
    user_t *user = ulist_search_bycfd(chat_msg->ctl.cfd);

    // 进行转发
    strcpy(chat_msg->from_id, user->id);
    strcpy(chat_msg->from_name, user->nickname);
    if (chat_msg->ctl.cmd == GROUP_CAHT)
    {
        // 群发
        ulist_t *uol = get_ulist();
        user_t *p = uol->head->next;
        while (p)
        {
            if (user == p)
            {
                p = p->next;
                continue;
            }
            send_msg(p->cfd, chat_msg, sizeof(chat_msg_t));
            p = p->next;
        }
        // 将用户消息保存
        FILE *fp_g = myfopen("Record/chat/group_chat.txt", "a");
        fprintf(fp_g, "%s(%s): %s\n", chat_msg->from_name, chat_msg->from_id, chat_msg->chat_buf);
        myfclose(fp_g);
    }
    else
    {
        user_t *user = ulist_search_byid(chat_msg->to_id);
        if (NULL == user)
        {
            strcpy(ser_msg.reply, "对方不在线！");
            send_msg(chat_msg->ctl.cfd, &ser_msg, sizeof(ser_msg_t));
            return;
        }
        send_msg(user->cfd, chat_msg, sizeof(chat_msg_t));
        FILE *fp_p = myfopen("Record/chat/private_chat.txt", "a");
        fprintf(fp_p, "From(%s)to(%s): %s\n", chat_msg->from_id, chat_msg->to_id, chat_msg->chat_buf);
        myfclose(fp_p);
    }
}

// 查看群文件
static void check_file_handler(ctl_t *ctl)
{
    system("ls Record/file > Record/files.txt");
    FILE *fp = myfopen("Record/files.txt", "r");
    ser_msg_t ser_msg;
    ser_msg.ctl.cfd = -1;
    ser_msg.ctl.msg_size = sizeof(ser_msg_t);


}

//查看在线用户
static void check_user_handler(ctl_t *user_msg)
{
    ser_msg_t ser_msg;
    ser_msg.ctl.msg_size = sizeof(ser_msg_t);
    ulist_t *ul = get_ulist();
    if(0 == ul->count)
    {
        ser_msg.ctl.cmd = 0;
        strcpy(ser_msg.reply, "没有在线用户！");
        send_msg(user_msg->cfd, &ser_msg, sizeof(ser_msg_t));
    }
    else{
        user_t *p = ul->head->next;
        while(p)
        {
            user_t *temp = p;
            ser_msg.ctl.cmd = 1;
            sprintf(ser_msg.reply, "%s\t\t%s", temp->id, temp->nickname);
            send_msg(user_msg->cfd, &ser_msg, sizeof(ser_msg_t));
            p = p->next;
            usleep(10000);
        }
        ser_msg.ctl.cmd = 0;
        sprintf(ser_msg.reply, "共%d位用户在线", ul->count);
        send_msg(user_msg->cfd, &ser_msg, sizeof(ser_msg_t));
    }
}

/******管理员处理函数******/
//管理员查看所有用户
static void check_all_handler(sql_msg_t *admin_msg)
{
    printf("%s\n",admin_msg->sql_buf);
    char **result;
    int row, col;
    ser_msg_t ser_msg;
    ser_msg.ctl.msg_size = sizeof(ser_msg_t);
    if(-1 == mysqlite_get_table(admin_msg->sql_buf, &result, &row, &col))
    {
        ser_msg.ctl.cmd = -1;
        strcpy(ser_msg.reply, "查看失败！");
        send_msg(admin_msg->ctl.cfd, &ser_msg, sizeof(ser_msg_t));
    }
    else
    {
        if(0 == row)
        {
            ser_msg.ctl.cmd = 0;
            strcpy(ser_msg.reply, "数据库为空！");
            send_msg(admin_msg->ctl.cfd, &ser_msg, sizeof(ser_msg_t));
        }
        else
        {
            ser_msg.ctl.cmd = 1;
            for(int i = 1; i <= row; i++)
            {
                ser_msg.ctl.cmd = 1;
                sprintf(ser_msg.reply, "%s\t%s\t%s\t%s\t%s\t%s\t%s", result[i*col],result[i*col+1],result[i*col+2],result[i*col+3],result[i*col+4],result[i*col+5],result[i*col+6]);
                send_msg(admin_msg->ctl.cfd, &ser_msg, sizeof(ser_msg_t));
                memset(&ser_msg.reply, 0, sizeof(ser_msg_t));  // 将服务器的回复全部置为0
            }
            ser_msg.ctl.cmd = 0;
            sprintf(ser_msg.reply, "共有%d位用户!", row);
            send_msg(admin_msg->ctl.cfd, &ser_msg, sizeof(ser_msg_t));  
        }
    }
}

// 管理员踢人下线
static void remove_user_handler(user_msg_t *remove_msg)
{
    user_t *p = ulist_search_byid(remove_msg->user_id);
    ser_msg_t ser_msg;
    ser_msg.ctl.cmd = -1;
    ser_msg.ctl.msg_size = sizeof(ser_msg_t);
    if(NULL == p)
    {
        strcpy(ser_msg.reply, "该用户不在线！");
        send_msg(remove_msg->ctl.cfd, &ser_msg, sizeof(ser_msg_t));
        return;
    }
    // 通知用户被踢下线
    ser_msg.ctl.cmd = REMOVE_USER;
    strcpy(ser_msg.reply, "你已被踢下线！");
    send_msg(p->cfd, &ser_msg, sizeof(ser_msg_t));
    // 通知管理员
    memset(ser_msg.reply, 0, sizeof(ser_msg_t));
    strcpy(ser_msg.reply, "该用户已被踢下线！");
    send_msg(remove_msg->ctl.cfd, &ser_msg, sizeof(ser_msg_t));
}

// 任务控制函数：根据操作指令调用相应的处理函数
void *task_routine(void *arg)
{
    ctl_t ctl = *(ctl_t *)arg;
    switch (ctl.cmd)
    {
    case REGISTER:
        reg_handler(arg);
        break;
    case LOGIN:
        login_handler(arg);
        break;
    case FIND_PASSWORD:
        find_psw_handler(arg);
        break;
    case PRIVATE_CHAT:
    case GROUP_CAHT:
        chat_handler(arg);
        break;
    case CHECK_FILE:
        check_file_handler(arg);
        break;
    case CHECK_ONLINE:
        check_user_handler(arg);
        break;
    case CHECK_ALL:
        check_all_handler(arg);
        break;
    case REMOVE_USER:
        remove_user_handler(arg);
        break;
    default:
        break;
    }
}