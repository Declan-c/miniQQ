#ifndef __INTERNET_H__
#define __INTERNET_H__
#define SIZE 1024

// 消息的传输协议
enum commond
{
    OPT_ERR = -1, // 操作有误，用于服务器回复客户端

    /*****用户操作*****/
    REGISTER = 0,  // 注册
    LOGIN,         // 登录
    FIND_PASSWORD, // 找回密码
    PRIVATE_CHAT,  // 私聊
    GROUP_CAHT,    // 群发
    CHECK_FILE,    // 查看群文件
    UPLOAD_FILE,   // 上传文件
    DOWNLOAD_FILE, // 下载文件
    UPDATE,        // 更新
    LEAVE_WORD,    // 留言
    LOOK_WORD,     // 查看留言
    CHECK_ONLINE,  // 查看在线用户
    /*****管理员操作*****/
    CHECK_ALL,   // 查看所有用户
    REMOVE_USER, // 踢人
    BAN,         // 禁言
    NOT_BAN,     // 解除禁言
};

// 消息的控制信息
typedef struct control
{
    int cmd;      // 命令
    int cfd;      // 客户端socket
    int msg_size; // 消息大小
} ctl_t;

// 用户信息
typedef struct information
{
    int age;           // 年龄
    char sex[10];       // 性别
    char myid[15];     // 账号ID（固定10字节）
    char tel[15];      // 电话号码
    char password[21]; // 密码--最大20字节
    char security[21]; // 密保--最大20字节
    char nickname[33]; // 昵称--最大32字节
} info_t;

/*****客户端消息结构定义*****/
// 与服务器数据库交互类信息：注册、登录、找回密码，修改
typedef struct
{
    ctl_t ctl;          // 控制信息
    char sql_buf[SIZE]; // 要执行的SQL语句
} sql_msg_t;

// 消息结构体
typedef struct
{
    ctl_t ctl;
    char from_id[11];    // 发送者ID
    char to_id[11];      // 接收消息的用户的ID, 群发置空
    char from_name[33];  // 发送者的昵称
    char chat_buf[SIZE]; // 消息内容
} chat_msg_t;

// 文件消息结构体
typedef struct
{
    ctl_t ctl;
    int flags; // 1代表发送的文件名, 0代表发送的文件
    long file_size;
    char filename[128];
    char from_id[11];
    char file_buf[SIZE]; // 消息内容
} file_msg_t;

// 管理员消息
typedef struct
{
    ctl_t ctl; // 控制信息
    char user_id[11];
    char sql_buf[SIZE]; // 要执行的SQL语句
} user_msg_t;

// 服务器消息
typedef struct server_message
{
    ctl_t ctl;        // 操作: -1代表操作失败(任意一个)
    char reply[SIZE]; // 服务器给客户端的消息
} ser_msg_t;

#endif