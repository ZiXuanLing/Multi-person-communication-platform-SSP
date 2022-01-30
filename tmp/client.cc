#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "proto/user_info_base.pb.h"
#include "proto/message_define.pb.h"

#include "common/mess_type.h"

char user_name[] = "hank";
char psssword[] = "hank1234";

char buffer[10244];

char data[10244];
int SetMessType(const char *mess_type)
{
    int len = strlen(mess_type);
    if (len != 3)
    {
        return -1;
    }
    /* Reserve three bytes of space for identification */
    data[0] = mess_type[0];
    data[1] = mess_type[1];
    data[2] = mess_type[2];

    return 0;
}
int GetMessType()
{
    int len = strlen(buffer);
    if (len != 3)
    {
        return -1;
    }
    return ((buffer[0] - '0') * 100 + (buffer[1] - '0') * 10 + buffer[2] - '0');
}
int main()
{
    // login
    ssp::UserInfoBase pb_user;
    // pb_user.set_user_id();
    pb_user.set_user_name(user_name);
    pb_user.set_password(psssword);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8999);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    int times = 10000;
    char data[10240];
    int ret = SetMessType("101");
    if (ret != 0)
    {
        return -1;
    }
    pb_user.SerializeToArray(data + 3, 10240); /* Serialized data */
    send(sock, data, sizeof(data), 0);
    while (times--)
    {
        int ret = recv(sock, buffer, sizeof(buffer), 0);
        if (ret < 0)
        {
            continue;
        }
        int mess_type = GetMessType();
        if (mess_type < 0)
        {
            return -1;
        }
        ssp::LoginRsp loginRsp;
        switch (mess_type)
        {
        case LOGIN_RSP:
            loginRsp.ParseFromArray(buffer + 3, 10240);
            break;
        }
    }
    return 0;
}