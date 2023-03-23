#include "client_func.h"

static info_t my_info;

// 设置账号
static void set_id(char *id)
{
    printf("账号: ");
    while (1)
    {
        scanf("%s", id);
        if (strlen(id) != 10)
        {
            printf("账号长度错误!\n");
        }
        else if (0 != strncmp("user", id, 4))
        {
            printf("账号格式错误!\n");
        }
        else
        {
            break;
        }
        printf("请重新输入：");
    }
}

// 设置密码
static void set_password(char *password)
{
    printf("密码: ");
    while (1)
    {
        scanf("%s", password);
        if (strlen(password) < 3)
        {
            printf("密码过短!\n");
        }
        else if (strlen(password) > 20)
        {
            printf("密码过长!\n");
        }
        else
        {
            break;
        }
        printf("请重新输入: ");
    }
}

// 设置昵称
static void set_nickname(char *nickname)
{
    printf("昵称: ");
    while (1)
    {
        scanf("%s", nickname);
        if (strlen(nickname) > 28)
        {
            printf("你的昵称过长!\n请重新输入: \n");
        }
        else
        {
            break;
        }
    }
}

// 注册时设置密码
static void set_reg_password(char *password)
{
    set_password(password);
    char confirm[21] = {0}; // 确认密码
    printf("确认密码: ");
    while (1)
    {
        scanf("%s", confirm);
        if (0 == strcmp(password, confirm))
        {
            break;
        }
        else
        {
            printf("密码确认有误! \n请重新输入: ");
        }
    }
}

// 设置密保
static void set_security(char *security)
{
    printf("密保: ");
    while (1)
    {
        scanf("%s", security);
        if (strlen(security) < 6)
        {
            printf("密保过短!\n");
        }
        else if (strlen(security) > 20)
        {
            printf("密保过长!\n");
        }
        else
        {
            break;
        }
        printf("请重新输入: ");
    }
}

// 设置电话号码
static void set_telephone(char *tel)
{
    printf("电话号码: ");
    while (1)
    {
        scanf("%s", tel);
        if ('1' != tel[0])
        {
            printf("电话号码格式有误!\n");
        }
        else if (strlen(tel) != 11)
        {
            printf("电话号码长度有误!\n");
        }
        else
        {
            break;
        }
        printf("请重新输入: ");
    }
}

// 设置性别
static void set_sex(char *sex)
{
    int flag = 1; // 默认为男
    printf("性别(1.男 0.女): ");
    scanf("%d", &flag);
    if (1 == flag)
    {
        strcpy(sex, "男");
    }
    else
    {
        strcpy(sex, "女");
    }
}

// 设置年龄
static void set_age(int *age)
{
    printf("年龄: ");
    while (1)
    {
        scanf("%d", age);
        if (age <= 0)
        {
            printf("年龄输入有误！");
        }
        else
        {
            break;
        }
        printf("请重新输入：");
    }
}

// 打印用户信息(全局变量my_info)
static void print_myinfo()
{
    printf("你的信息如下:\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("            昵称:   %s\n", my_info.nickname);
    printf("            密码:   %s\n", my_info.password);
    printf("            密保:   %s\n", my_info.security);
    printf("            年龄:   %d\n", my_info.age);
    printf("            性别:   %s\n", my_info.sex);
    printf("        电话号码:   %s\n", my_info.tel);
    printf("----------------------------------------------------------------------------------------\n");
}

// 注册信息
static sql_msg_t client_register()
{
    system("clear");
    printf("----------------------------------------------------------------------------------------\n");
    printf("                                  欢迎注册chatroom\n");
    printf("    温馨提示:\n");
    printf("            1、账号为10位数字,请牢记自己的账号密码\n");
    printf("            2、密码密保最少5位最多20位,密码由字母下划线数字组成且不能数字开头\n");
    printf("            3、注册成功后要再重新登录\n");
    printf("----------------------------------------------------------------------------------------\n");
    // 注册信息
    memset(&my_info, 0, sizeof(info_t));
    printf("请输入信息完成注册:\n");
    set_id(my_info.myid);
    set_nickname(my_info.nickname);
    set_reg_password(my_info.password);
    set_security(my_info.security);
    set_sex(my_info.sex);
    set_telephone(my_info.tel);
    set_age(&my_info.age);
    while (1)
    {
        print_myinfo();
        printf("\n确认信息是否有误,无误即发送(1.有 0.无): ");
        int confirm;
        scanf("%d", &confirm);
        if (0 == confirm) // 信息无误，发送至服务器注册
        {
            sql_msg_t reg;
            // 设置注册的请求信息
            reg.ctl.cmd = REGISTER;               // 注册
            reg.ctl.msg_size = sizeof(sql_msg_t); // 消息大小
            reg.ctl.cfd = -1;                     // 客户端置-1,发送过去服务器设置
            // 封装sql语句
            sprintf(reg.sql_buf, "insert into user values('%s', '%s', '%s', '%s','%s', '%s', %d);", my_info.myid, my_info.nickname, my_info.password, my_info.security, my_info.tel, my_info.sex, my_info.age);
            // 发送注册请求
            return reg;
        }
    }
}

// 登录
static sql_msg_t client_login()
{
    system("clear");
    printf("----------------------------------------------------------------------------------------\n");
    printf("                                  欢迎登录chatroom\n");
    printf("----------------------------------------------------------------------------------------\n");

    char id[11] = {0};
    char password[21] = {0};
    set_id(id);
    set_password(password);

    int confirm;
    printf("\n确认登录(0.登录 1.不登录):   ");
    scanf("%d", &confirm);
    if (0 == confirm) // 确认登陆
    {
        sql_msg_t login;
        login.ctl.cmd = LOGIN;                  // 设置登录操作
        login.ctl.msg_size = sizeof(sql_msg_t); // 设置消息大小
        login.ctl.cfd = -1;
        // 封装登录的sql语句 -- select * from user where id = '' and password = ''
        sprintf(login.sql_buf, "select * from user where id = '%s' and password = '%s';", id, password);
        return login;
    }
    else // 取消操作，退出
    {
        system("clear");
        return;
    }
}

// 找回密码
static sql_msg_t client_find_psd()
{
    system("clear");
    printf("----------------------------------------------------------------------------------------\n");
    printf("                                  找回chatroom账号\n");
    printf("----------------------------------------------------------------------------------------\n");

    char id[11] = {0};
    char security[21] = {0};
    set_id(id);
    set_security(security);

    int confirm;
    printf("\n确认发送(0.发送 1.不发送):   ");
    scanf("%d", &confirm);
    if (0 == confirm) // 确认发送
    {
        sql_msg_t find_psd;
        find_psd.ctl.cfd = -1;
        find_psd.ctl.cmd = FIND_PASSWORD;
        find_psd.ctl.msg_size = sizeof(sql_msg_t);
        // 封装找回密码的sql语句
        sprintf(find_psd.sql_buf, "select password from user where id = '%s' and sec = '%s';", id, security);
        return find_psd;
    }
    else // 取消操作，退出
    {
        system("clear");
        return;
    }
}

// 首页菜单
static void show_menu()
{
    system("clear");
    printf("----------------------------------------------------------------------------------------\n");
    printf("                                  欢迎使用chatroom\n\n");
    printf("                         1.登录  2.注册  3.忘记密码  0.退出\n");
    printf("----------------------------------------------------------------------------------------\n");
}

// 首页操作函数
static sql_msg_t show_func()
{
    int choice;
    sql_msg_t temp_msg = {0};
    while (1)
    {
        printf(">>>");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            temp_msg = client_login();
            break;
        case 2:
            temp_msg = client_register();
            break;
        case 3:
            temp_msg = client_find_psd();
            break;
        case 0:
            exit(0);
        default:
            printf("指令有误，请重新输入！");
            continue;
        }
        break;
    }
    return temp_msg;
}

// 首页
void show(const char *ip, int port)
{
    ser_msg_t ser_msg;
    show_menu();
    sql_msg_t msg = show_func(); // 获取用户操作选择
    client_connect(ip, port);    // 连接服务器
    while (1)
    {
        client_send(&msg, sizeof(sql_msg_t)); // 发送消息
        // 接收消息
        memset(&ser_msg, 0, sizeof(ser_msg_t));
        client_recv(&ser_msg, sizeof(ser_msg_t));
        // 登录成功，进入主页
        if (LOGIN == ser_msg.ctl.cmd)
        {
            printf("登录成功\n");
            memcpy(&my_info, ser_msg.reply, sizeof(info_t));
            break;
        }
        else
        {
            show_menu();
            if (OPT_ERR == ser_msg.ctl.cmd)
            {
                printf("(%s)操作有误！请重新操作！\n", ser_msg.reply);
            }
            else if (REGISTER == ser_msg.ctl.cmd)
            {
                printf("注册成功，请重新登录！\n");
            }
            else if (FIND_PASSWORD == ser_msg.ctl.cmd)
            {
                printf("你的密码为：%s，请重新登录！\n", ser_msg.reply);
            }
        }
        memset(&msg, 0, sizeof(sql_msg_t));
        msg = show_func();
    }
}

/******主页操作******/
// 创建保存消息和文件的目录
static void folder_create()
{
    char buf[100] = {0};
    sprintf(buf, "Record/%s", my_info.myid);
    // 先判断是否有保存信息的文件夹
    if (access(buf, F_OK) == 0) // 文件存在
    {
        return;
    }
    // 创建主文件夹，使用用户ID命名
    sprintf(buf, "mkdir Record/%s", my_info.myid);
    system(buf);
    // 创建自己文件: chat和 file
    sprintf(buf, "mkdir Record/%s/chat Record/%s/file", my_info.myid, my_info.myid);
    system(buf);
}

// 聊天功能菜单
static void chat_menu()
{
    system("clear");
    printf("----------------------------------------------------------------------------------------\n");
    printf("                                       chatroom聊天\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("                           1.群聊  2.私聊  3.查看在线用户  0.返回\n");
    printf("----------------------------------------------------------------------------------------\n");
}

// 用户发送消息, flags标志是群发（GROUP_CHAT）私聊（PRIVATE_CHAT）
static void user_chat(const char *user, int flags)
{
    chat_msg_t chat_msg;
    chat_msg.ctl.cfd = -1;
    chat_msg.ctl.cmd = flags;
    chat_msg.ctl.msg_size = sizeof(chat_msg_t);
    memset(chat_msg.from_id, 0, sizeof(chat_msg.from_id));
    memset(chat_msg.from_name, 0, sizeof(chat_msg.from_name));
    strcpy(chat_msg.to_id, user);

    char chat_path[128] = {0}; // 聊天文件存储路径
    if (PRIVATE_CHAT == flags) // 私聊
    {
        // 聊天记录保存: user000000发送给user000001的消息：Record/user000000/chat/user000001.txt
        sprintf(chat_path, "Record/%s/chat/%s.txt", my_info.myid, user);
    }
    else if (GROUP_CAHT == flags) // 群聊
    {
        // 聊天记录保存: group_chat.txt
        sprintf(chat_path, "Record/%s/chat/group_chat.txt", my_info.myid);
    }
    FILE *fp = myfopen(chat_path, "a");
    printf("键入(bye)结束本次消息发送!\n");
    while (1)
    {
        memset(chat_msg.chat_buf, 0, sizeof(chat_msg.chat_buf));
        scanf("%s", chat_msg.chat_buf);
        if (0 == strcmp("bye", chat_msg.chat_buf)) // bye: 退出本次聊天的结束语
        {
            break;
        }
        else
        {
            // 私聊群发保存格式
            fprintf(fp, "我发的消息:  %s\n", chat_msg.chat_buf);
        }
        // 发送信息
        client_send(&chat_msg, sizeof(chat_msg_t));
    }
    myfclose(fp);
}

// 查看在线用户
static void check_online_func()
{
    ctl_t user_check;
    user_check.cfd = -1;
    user_check.cmd = CHECK_ONLINE;
    user_check.msg_size = sizeof(ctl_t);
    client_send(&user_check, sizeof(ctl_t));
}

static void chat_func()
{
    chat_menu();
    int choice;
    while (1)
    {
        printf(">>>");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1: // 群聊
            user_chat("群聊", GROUP_CAHT);
            break;
        case 2: // 私聊
            char user[11];
            printf("请输入对方的");
            set_id(user);
            user_chat(user, PRIVATE_CHAT); 
            break;
        case 3:
            check_online_func();
            break;
        case 0:
            return;
        default:
            chat_menu();
            continue;
        }
    }
}

// 文件功能菜单
static void file_menu()
{
    system("clear");
    printf("----------------------------------------------------------------------------------------\n");
    printf("                                       chatroom文件\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("                         1.查看群文件  2.下载文件  3.上传文件  0.返回\n");
    printf("----------------------------------------------------------------------------------------\n");
}

// 查看文件
static void check_file_func()
{
    // 初始化要发送的消息
    ctl_t check_file;
    check_file.cfd = -1;
    check_file.cmd = CHECK_FILE;
    check_file.msg_size = sizeof(ctl_t);
    // 给服务器发送请求
    client_send(&check_file, sizeof(ctl_t));
}

// 上传文件
static void upload_file_func()
{
}

// 下载文件
static void download_file_func()
{
}

// 文件功能实现
static void file_func()
{
    file_menu();
    int choice;
    while (1)
    {
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            check_file_func();
            break;
        case 2:
            upload_file_func();
            break;
        case 3:
            download_file_func();
            break;
        case 0:
            return;
        default:
            file_menu();
            continue;
            ;
        }
    }
}

// 个人管理功能的菜单
static void myself_menu()
{
    system("clear");
    printf("----------------------------------------------------------------------------------------\n");
    printf("                                      chatroom个人管理\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("                      1.查看个人信息  2.修改个人信息  3.查看聊天记录  0.返回\n");
    printf("----------------------------------------------------------------------------------------\n");
}

// 修改功能的菜单
static void update_menu()
{
    system("clear");
    printf("----------------------------------------------------------------------------------------\n");
    printf("                                     修改个人信息\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("                      1.昵称 2.密码 3.密保 4.号码 5.性别 6.年龄 0.返回\n");
    printf("----------------------------------------------------------------------------------------\n");
}

// 修改个人信息
static void update_myinfo()
{
    update_menu();
    int choice;
    while (1)
    {
        scanf("%d", &choice);
        if (0 == choice)
        {
            break;
        }
        else if (choice > 6 || choice < 0)
        {
            update_menu();
        }
        else
        {
            choice -= 1;
            char *my_buf[6] = {my_info.nickname, my_info.password, my_info.security, my_info.tel, my_info.sex};
            void (*set_func[6])(char *buf) = {set_nickname, set_password, set_security, set_telephone, set_sex};
            set_func[choice](my_buf[choice]);
        }
        // 同步到server
    }
}

// 查看聊天记录
static void check_record_func()
{
    char chat_path[128] = {0};
}

// 个人管理功能操作函数
static void myself_func()
{
    myself_menu();
    int choice;
    while (1)
    {
        printf(">>>");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            print_myinfo();
            break;
        case 2:
            update_myinfo();
            break;
        case 3:
            check_record_func();
            break;
        case 0:
            return;
        default:
            myself_menu();
            continue;
        }
    }
}

// 主页菜单
static void home_menu()
{
    system("clear");
    printf("----------------------------------------------------------------------------------------\n");
    printf("                                       chatroom\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("                            1.聊天  2.文件  3.个人  0.退出\n");
    printf("----------------------------------------------------------------------------------------\n");
}

// 次线程处理函数：接收并处理消息
static void *msg_routine(void *arg)
{
    char chat_buf[1024] = {0};
    char chat_path[128] = {0};
    while (1)
    {
        memset(chat_buf, 0, sizeof(chat_buf));
        client_recv(chat_buf,sizeof(chat_buf)); // 从服务端接收消息
        ctl_t *ctl = (ctl_t *)chat_buf;
        if(ctl->cmd == PRIVATE_CHAT || ctl->cmd == GROUP_CAHT)
        {
            chat_msg_t *chat_msg = (chat_msg_t *)chat_buf;
            if(chat_msg->ctl.cmd == PRIVATE_CHAT)
            {
                sprintf(chat_path, "Record/%s/chat/%s.txt", my_info.myid, chat_msg->from_id);
                printf("%s(%s):(私聊)\n %s\n", chat_msg->from_name, chat_msg->from_id, chat_msg->chat_buf);
            }
            else
            {
                sprintf(chat_path, "Record/%s/chat/group_chat.txt", my_info.myid);
                printf("%s(%s):(群聊)\n %s\n", chat_msg->from_name, chat_msg->from_id, chat_msg->chat_buf);
            }
            FILE *fp = myfopen(chat_path, "a");
            fprintf(fp,"%s(%s):\n %s\n",chat_msg->from_name, chat_msg->from_id, chat_msg->chat_buf);
            myfclose(fp);
        }
        else if(DOWNLOAD_FILE == ctl->cmd)
        {
            file_msg_t *file_msg = (file_msg_t *)chat_buf;
            char file_path[256] = {0};
            sprintf(file_path, "Record/%s/file/%s", my_info.myid, file_msg->filename);
            FILE *fp = myfopen(file_path, "a+");
            myfwrite(fp, file_msg->file_buf, strlen(file_msg->file_buf));
            myfclose(fp);
        }
        else        //其他不需处理的消息
        {
            ser_msg_t *ser_msg = (ser_msg_t *)chat_buf;
            printf("%s\n", ser_msg->reply);
            if(ser_msg->ctl.cmd == REMOVE_USER)     //被管理员踢下线
            {
                exit(0);
            }
        }
    }
}

// 主页操作
void home()
{
    // 创建消息记录文件目录
    folder_create();
    pthread_t msg_tid; // 创建一个次线程
    if (0 != pthread_create(&msg_tid, NULL, msg_routine, NULL))
    {
        ERRLOG("pthread_create");
        exit(0);
    }
    int choice;
    while (1)
    {
        home_menu();
        printf(">>>");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            chat_func();
            break;
        case 2:
            file_func();
            break;
        case 3:
            myself_func();
            break;
        case 0:
            pthread_cancel(msg_tid);
            pthread_join(msg_tid, NULL);
            exit(0);
            break;
        default:
            home_menu();
            continue;
        }
    }
}