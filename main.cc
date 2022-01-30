/**
 * @file main.cc
 * @author liuhao (2226958871@qq.com)
 * @brief 
 * @version 0.0.1
 * @date 2022-01-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "UserManager.h"
#include "RelationManager.h"
#include "MessageManager.h"
#include "PhotoManager.h"

#include "proto/message_define.pb.h"

#include <stdio.h>

#include <unistd.h>
#include <sys/time.h>
#include <netinet/in.h>
#include "arpa/inet.h"
#include "sys/socket.h"

#include "common/mess_type.h"
#include "common/ret_value.h"

UserManager user_svr;
RelationManager rela_svr;
MessageManager mess_svr;
PhotoManager photo_svr;

/**
 * @brief
 *
 * @param buffer
 * @return int
 */
int sizeof_b(unsigned char *buffer)
{
    for (int i = 0; i < 10244; i++)
    {
        if (buffer[i] == 0)
        {
            return i;
        }
    }
    return 10244;
}

unsigned char recv_buffer[10244];

/**
 * @brief Get the Req Mess Type object
 *
 * @return int
 */
int GetReqMessType()
{
    return ((recv_buffer[0] - '0') * 100 + (recv_buffer[1] - '0') * 10 + recv_buffer[2] - '0');
}

int mess_type = -1;

unsigned char send_buffer[10244];

int server_sock;
struct sockaddr_in server_addr;

void SocketInit()
{
    server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8999);
    bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_sock, 10000);
    printf("Socket Inited\n");
}

int no_recv_count = 0;
struct sockaddr_in clnt_addr;
int clnt_sock;
int accpet_flag = 0;
int SocketAccept()
{
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(server_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    accpet_flag = 1;
    printf("Socket Accepted\n");
    return 0;
}

int send_fail_count = 0;
int ClientClose()
{
    send_fail_count = 1;
#ifdef _D
    printf("Client Disconnet and Closed\n");
    fflush(stdout);
#endif
    close(clnt_sock);
    accpet_flag = 0;
    return 0;
}

int GetMessType()
{
    int len = strlen((char *)recv_buffer);
    if (len != 3)
    {
        return -1;
    }
    return ((recv_buffer[0] - '0') * 100 + (recv_buffer[1] - '0') * 10 + recv_buffer[2] - '0');
}

int client_sock;

int SocketCheckRecv()
{
    if (accpet_flag == 0)
    {
        return 0;
    }
    int ret = recv(clnt_sock, recv_buffer, 10244, 0);
    if (ret > 0)
    {
        mess_type = GetReqMessType();
#ifdef _D
        printf("mess_type:%d\n", mess_type);
        fflush(stdout);
#endif // DEBUG

        fflush(stdout);
        return 0;
    }
    else
    {
        no_recv_count++;
        if (no_recv_count > 0)
        {
            ClientClose();
        }
    }
    return -1;
}

int SocketSendRsp()
{
    int send_size = sizeof_b(send_buffer);
    int ret = send(clnt_sock, send_buffer, send_size, 0);
    if (ret < 0)
    {
        ClientClose();
    }
    return 0;
}

int SocketClose()
{
    close(server_sock);
    return 0;
}

int SetRspMessType(char *mess_type)
{
    if (strlen(mess_type) >= 3)
    {
        send_buffer[0] = mess_type[0];
        send_buffer[1] = mess_type[1];
        send_buffer[2] = mess_type[2];
    }
    return 0;
}

ssp::RegReq msgRegReq;
ssp::RegRsp msgRegRsp;

ssp::LoginReq msgLoginReq;
ssp::LoginRsp msgLoginRsp;

ssp::LogoutReq msgLogoutReq;
ssp::LogoutRsp msgLogoutRsp;

ssp::PublishMessageReq msgPublishMessageReq;
ssp::PublishMessageRsp msgPublishMessageRsp;

ssp::GetMessageReq msgGetMessageReq;
ssp::GetMessageRsp msgGetMessageRsp;

ssp::GetPhotoReq msgGetPhotoReq;
ssp::GetPhotoRsp msgGetPhotoRsp;

ssp::AddFriendReq msgAddFriendReq;
ssp::AddFriendRsp msgAddFriendRsp;

ssp::DelFriendReq msgDelFriendReq;
ssp::DelFriendRsp msgDelFriendRsp;

ssp::AddBlackReq msgAddBlackReq;
ssp::AddBlackRsp msgAddBlackRsp;

ssp::DelBlackReq msgDelBlackReq;
ssp::DelBlackRsp msgDelBlackRsp;

char mess_type_str[10];
int main()
{
    user_svr.Start();
    rela_svr.Start();
    mess_svr.Start();
    photo_svr.Start();
    SocketInit();

    SocketAccept();
    int times = 0;
    int server_on = 1;
    while (server_on)
    {
        time_t now = time(NULL);
        SocketCheckRecv();
        int ret = 0;
        if (mess_type < 0)
        {
            continue;
        }
        times++;
#ifdef _D
        printf("message count: %d\n", times);
        fflush(stdout);
#endif
        switch (mess_type)
        {
        case REG_REQ: // Receive registration request
        {
            msgRegReq.ParseFromArray(recv_buffer + 3, 10240); // Parse the message
            // Create a user
            ret = user_svr.CreateUser(
                msgRegReq.user_name().data(),
                msgRegReq.password().data(),
                msgRegReq.from(),
                now);
            msgRegRsp.set_ret(ret);
            if (ret == SUCCESS)
            {
                int user_id = user_svr.GetUserIdByUserName(msgRegReq.user_name().data());
                // rela_svr.UserRelationInit(user_id); /* Assign a relational structure to a newly registered user */
                msgRegRsp.set_user_id(user_id);
            }
#ifdef _D
            printf("402 msgRegRsp ret:%d\n", ret);
            fflush(stdout);
#endif
            sprintf(mess_type_str, "%d", REG_RSP);
            SetRspMessType(mess_type_str);
            msgRegRsp.SerializeToArray(send_buffer + 3, 10240); /* reply */

            SocketSendRsp();
            if (ret < 0)
            {
                server_on = 0;
            }
            break;
        }
        case LOGIN_REQ:
        {
            msgLoginReq.ParseFromArray(recv_buffer + 3, 10240);
            ret = user_svr.LoginCheck(msgLoginReq.user_name().c_str(), msgLoginReq.password().c_str());
            if (ret == SUCCESS)
            {
                // update login time
                int user_id = user_svr.GetUserIdByUserName(msgLoginReq.user_name().data());
                user_svr.UpdateUserLoginTime(user_id, now);
                msgLoginRsp.set_user_id(user_id);
                msgLoginRsp.set_ret(ret);
            }
            else
            {
                msgLoginRsp.set_ret(ret);
            }
#ifdef _D
            printf("[DEBUG   ]RegRsp ret:%d uid:%d\n", ret, msgRegRsp.user_id());
            fflush(stdout);
#endif // DEBUG
            sprintf(mess_type_str, "%d", LOGIN_RSP);
            SetRspMessType(mess_type_str);
            msgRegRsp.SerializeToArray(send_buffer + 3, 10240);
            SocketSendRsp();
        }
        case ADD_FRIEND_REQ:
        {
            msgAddFriendReq.ParseFromArray(recv_buffer + 3, 10240);
#ifdef _D
            printf("[DEBUG   ]login uname: %s\n", msgLoginReq.user_name().c_str());
#endif
            ret = user_svr.CheckExist(msgAddFriendReq.user_id());
            if (ret == USER_EXIST)
            {
                ret = user_svr.CheckExist(msgAddFriendReq.other_id());
                if (ret == USER_EXIST)
                {
                    rela_svr.AddFriend(msgAddFriendReq.user_id(), msgAddFriendReq.other_id());
                    msgAddFriendRsp.set_ret(ret);
                }
                msgAddFriendRsp.set_ret(ret);
            }
            else
            {
                msgAddFriendRsp.set_ret(ret);
            }
#ifdef _D
            printf("[DEBUG   ]ret:%d\n", ret);
            fflush(stdout);
#endif
            sprintf(mess_type_str, "%d", ADD_FRIEND_RSP);
            SetRspMessType(mess_type_str);
            msgRegRsp.SerializeToArray(send_buffer + 3, 10240);
            SocketSendRsp();
            break;
        }
        case ADD_BLACK_REQ:
        {
            msgAddBlackReq.ParseFromArray(recv_buffer + 3, 10240);
#ifdef _D
            printf("[DEBUG   ]AddFriend, uid1:%d uid2:%d\n",
					   msgAddFriendReq.user_id(), msgAddFriendReq.other_id());
#endif // DEBUG
            ret = user_svr.CheckExist(msgAddBlackReq.user_id());
            if (ret == USER_EXIST)
            {
                ret = user_svr.CheckExist(msgAddBlackReq.other_id());
                if (ret == USER_EXIST)
                {
                    rela_svr.AddBlack(msgAddBlackReq.user_id(), msgAddBlackReq.other_id());
                    msgAddBlackRsp.set_ret(ret);
                }
                msgAddBlackRsp.set_ret(ret);
            }
            else
            {
                msgAddBlackRsp.set_ret(ret);
            }
#ifdef _D
            printf("[DEBUG   ]302 AddFriendRsp ret:%d\n", ret);
            fflush(stdout);
#endif
            sprintf(mess_type_str, "%d", ADD_BLACK_RSP);
            SetRspMessType(mess_type_str);
            msgAddBlackRsp.SerializeToArray(send_buffer + 3, 10240);
            SocketSendRsp();
            break;
        }
        case PUBLISH_MESSAGE_REQ:
        {
            msgPublishMessageReq.ParseFromArray(recv_buffer + 3, 10240);
#ifdef _D
            printf("[DEBUG   ]mess_type:303, uid1:%d uid2:%d\n",
                    msgDelFriendReq.user_id(), msgDelFriendReq.other_id());
#endif // DEBUG
            if (ret == USER_EXIST)
            {
                MessageInfo message;
                message.set_message_id(1); // todo get from mysql
                message.set_user_id(msgPublishMessageReq.user_id());
                message.set_publish_time(now);
                ret = mess_svr.PushMessage(message);
                RelationInfo *rela_info = rela_svr.GetRelation(msgPublishMessageReq.user_id());
                for (int i = 0; i < rela_info->friend_count(); i++)
                {
                    photo_svr.UpdatePhoto(
                        rela_info->GetFriendUserIdByIndex(i),
                        msgPublishMessageReq.user_id(),
                        now,
                        message.message_id());
                }
                msgPublishMessageRsp.set_ret(SUCCESS);
            }
            else
            {
                msgPublishMessageRsp.set_ret(ret);
            }
#ifdef _D
            printf("[DEBUG   ]304 DelFriendRsp ret:%d\n", ret);
#endif
            sprintf(mess_type_str, "%d", ADD_BLACK_RSP);
            SetRspMessType(mess_type_str);
            msgPublishMessageRsp.SerializeToArray(send_buffer + 3, 10240);
#ifdef _D
            printf("[DEBUG   ]ret_byte_size:%d\n", ret);
#endif // DEBUG
            SocketSendRsp();
            break;
        }
        case GET_PHOTO_REQ:
        {
            msgGetPhotoReq.ParseFromArray(recv_buffer + 3, 10240);
#ifdef _D

#endif // DEBUG
            ret = user_svr.CheckExist(msgGetPhotoReq.user_id());
            if (ret == USER_EXIST)
            {
                PhotoInfo *photo = photo_svr.GetPhoto(msgGetPhotoReq.user_id());
                if (photo != NULL)
                {
                    MessageInfo *message = mess_svr.GetMessage(photo->last_publish_message_id());
                    if (message != NULL)
                    {
                        msgGetPhotoRsp.mutable_last_message()->set_message_id(message->message_id());
                        msgGetPhotoRsp.mutable_last_message()->set_content(message->content());
                        msgGetPhotoRsp.mutable_last_message()->set_publisher(photo->last_publisher());
                        msgGetPhotoRsp.mutable_last_message()->set_publish_time(photo->last_publish_time());
                        msgGetPhotoRsp.set_ret(SUCCESS);
                    }
                    else
                    {
                        msgGetPhotoRsp.set_ret(MESSAGE_NOT_EXIST);
                    }
                }
                else
                {
                    msgGetPhotoRsp.set_ret(PHOTO_NOT_EXIST);
                }
            }
            else
            {
                msgGetPhotoRsp.set_ret(ret);
            }
#ifdef _D

#endif
            sprintf(mess_type_str, "%d", ADD_BLACK_RSP);
            SetRspMessType(mess_type_str);
            msgGetPhotoRsp.SerializeToArray(send_buffer + 3, 10240);
            SocketSendRsp();
            break;
        }
        case GET_MESSAGE_LIST_REQ:
        {
            msgGetMessageReq.ParseFromArray(recv_buffer + 3, 10240);
            ret = user_svr.CheckExist(msgGetMessageReq.user_id());
            if (ret == USER_EXIST)
            {
                user_svr.UpdateUserFreshTime(msgGetMessageReq.user_id(), now);
                msgGetMessageRsp.set_ret(SUCCESS);
            }
            else
            {
                msgGetMessageRsp.set_ret(ret);
            }
#ifdef _D

#endif
            sprintf(mess_type_str, "%d", ADD_BLACK_RSP);
            SetRspMessType(mess_type_str);
            msgGetMessageRsp.SerializeToArray(send_buffer + 3, 10240);
            SocketSendRsp();
            break;
        }
        case DEL_FRIEND_REQ:
        {
            msgDelFriendReq.ParseFromArray(recv_buffer + 3, 10240);
            ret = user_svr.CheckExist(msgDelFriendReq.user_id());
            if (ret == USER_EXIST)
            {
                ret = user_svr.CheckExist(msgDelFriendReq.other_id());
                if (ret == USER_EXIST)
                {
                    ret = rela_svr.DeleteFriend(msgDelFriendReq.user_id(), msgDelFriendReq.other_id());
                    msgDelFriendRsp.set_ret(ret);
                }
                else
                {
                    msgDelFriendRsp.set_ret(ret);
                }
            }
            else
            {
                msgDelFriendRsp.set_ret(ret);
            }
#ifdef _D

#endif
            sprintf(mess_type_str, "%d", ADD_BLACK_RSP);
            SetRspMessType(mess_type_str);
            msgDelFriendRsp.SerializeToArray(send_buffer + 3, 10240);
            SocketSendRsp();
            break;
        }
        case DEL_BLACK_REQ:
        {
            msgDelBlackReq.ParseFromArray(recv_buffer + 3, 10240);
            ret = user_svr.CheckExist(msgDelBlackReq.user_id());
            if (ret == USER_EXIST)
            {
                ret = user_svr.CheckExist(msgDelBlackReq.other_id());
                if (ret == USER_EXIST)
                {
                    ret = rela_svr.DeleteBlack(msgDelBlackReq.user_id(), msgDelBlackReq.other_id());
                    msgDelBlackRsp.set_ret(ret);
                }
                else
                {
                    msgDelBlackRsp.set_ret(ret);
                }
            }
            else
            {
                msgDelBlackRsp.set_ret(ret);
            }
#ifdef _D

#endif
            sprintf(mess_type_str, "%d", ADD_BLACK_RSP);
            SetRspMessType(mess_type_str);
            msgDelBlackRsp.SerializeToArray(send_buffer + 3, 10240);
            SocketSendRsp();
            break;
        }
        case LOGOUT_REQ:
        {
            msgLogoutReq.ParseFromArray(recv_buffer + 3, 10240);
            ret = user_svr.CheckExist(msgLogoutReq.user_id());
            if (ret == USER_EXIST)
            {
                user_svr.UserLogout(msgLogoutReq.user_id(), now);
                msgLogoutRsp.set_ret(ret);
            }
            else
            {
                msgLogoutRsp.set_ret(ret);
            }
#ifdef _D

#endif
            sprintf(mess_type_str, "%d", ADD_BLACK_RSP);
            SetRspMessType(mess_type_str);
            msgLogoutRsp.SerializeToArray(send_buffer + 3, 10240);
            SocketSendRsp();
            break;
        }
        default:
#ifdef _D
            printf("unknown mess_type: %d\n", mess_type);
#endif // DEBUG
            break;
        }
        user_svr.Proc();
        rela_svr.Proc();
        mess_svr.Proc();
        photo_svr.Proc();
        mess_type = -1;

        // usleep(5000);
    }

    user_svr.Shutdown();
    rela_svr.Shutdown();
    mess_svr.Shutdown();
    photo_svr.Shutdown();
    return 0;
}