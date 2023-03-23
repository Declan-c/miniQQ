#include "tcp_client.h"
#include "client_func.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#define IP "192.168.202.128"
#define PORT 6666

int main(int argc, char const *argv[])
{
    //首页
    show(IP, PORT);

    //主页
    home();
    return 0;
}