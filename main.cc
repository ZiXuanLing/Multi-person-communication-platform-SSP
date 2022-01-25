#include "UserManager.h"
#include "RelationManager.h"
#include "MessageManager.h"
#include "PhotoManager.h"

#include "common/mess_type.h"

#include "proto/message_define.pb.h"

#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "arpa/inet.h"
#include "sys/socket.h"

UserManager user_svr;
RelationManager rela_svr;
MessageManager mess_svr;
PhotoManager photo_svr;

char buffer[10244];

int GetMessType() {
    int len = strlen(buffer);
    if (len != 3) {
        return -1;
    }
    return ((buffer[0] - '0') * 100 + (buffer[1] - '0') * 10 + buffer[2] - '0');
}

void Response(ssp::LoginRsp) {
    // 
}

int serv_sock;
struct sockaddr_in serv_addr;
int client_sock;
int SocketInit() {
    serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8999);
    bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

}

int mess_type = -1;

int SocketCheckRecv() {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    client_sock = accept(serv_sock, (struct sockaddr*)& client_addr, &client_len);
    int ret = recv(serv_sock, buffer, sizeof(buffer) - 1, 0);
    if (ret > 0) {
        mess_type = GetMessType();
    }
}

int main() {
    user_svr.Start();
    rela_svr.Start();
    mess_svr.Start();
    photo_svr.Start();
    SocketInit();
    while (1) {
        time_t now = time(NULL);
        SocketCheckRecv();
        int ret = 0;
        if (mess_type < 0) {
            continue;
        }
        switch (GetMessType()) {
            case REG_REQ: {// register request
                ssp::RegReq regReq;
                ssp::RegRsp regRsp;
                regReq.ParseFromArray(buffer + 3, 10240);
                ret = user_svr.CreateUser(regReq.user_name().data(), regReq.password().data(), regReq.from());
                regRsp.set_ret(ret);
                if (ret == SUCCESS) {
                    int user_id = user_svr.GetUserIdByUserName(regReq.user_name().data());
                    regRsp.set_user_id(user_id);
                }
                break;
            }
            case LOGIN_REQ: {
                ssp::LoginReq loginReq;
                ssp::LoginRsp loginRsp;
                loginReq.ParseFromArray(buffer + 3, 10240);
                ret = user_svr.LoginCheck(loginReq.user_name().data(), loginReq.password().data());
                if (ret == SUCCESS) {
                    // update login time
                    int user_id = user_svr.GetUserIdByUserName(loginReq.user_name().data());
                    user_svr.UpdateUserLoginTime(user_id, now);
                    loginRsp.set_user_id(user_id);
                    loginRsp.set_ret(ret);
                }
                else {
                    loginRsp.set_ret(ret);
                }
            }
            case ADD_FRIEND_REQ: {
                ssp::AddFriendReq addFriendReq;
                ssp::AddFriendRsp addFriendRsp;
                addFriendReq.ParseFromArray(buffer + 3, 10240);
                ret = user_svr.CheckExist(addFriendReq.user_id());
                if (ret == USER_EXIST) {
                    ret = user_svr.CheckExist(addFriendReq.other_id());
                    if (ret == USER_EXIST) {
                        rela_svr.AddFriend(addFriendReq.user_id(), addFriendReq.other_id());
                        addFriendRsp.set_ret(ret);
                    }
                    addFriendRsp.set_ret(ret);
                }
                else {
                    addFriendRsp.set_ret(ret);
                }
                break;
            }
            case  ADD_BLACK_REQ: {
                ssp::AddBlackReq addBlackReq;
                ssp::AddBlackRsp addBlackRsp;
                addBlackReq.ParseFromArray(buffer + 3, 10240);
                ret = user_svr.CheckExist(addBlackReq.user_id());
                if (ret == USER_EXIST) {
                    ret = user_svr.CheckExist(addBlackReq.other_id());
                    if (ret == USER_EXIST) {
                        rela_svr.AddBlack(addBlackReq.user_id(), addBlackReq.other_id());
                        addBlackRsp.set_ret(ret);
                    }
                    addBlackRsp.set_ret(ret);
                }
                else {
                    addBlackRsp.set_ret(ret);
                }
                break;
            }
            case PUBLISH_MESSAGE_REQ: {
                ssp::PublishMessageReq publishReq; // request
                ssp::PublishMessageRsp publishRsp; // reply
                publishReq.ParseFromArray(buffer + 3, 10240);
                if (ret == USER_EXIST) {
                    MessageInfo message;
                    message.set_message_id(1); // todo get from mysql
                    message.set_user_id(publishReq.user_id());
                    message.set_publish_time(now);
                    ret = mess_svr.PushMessage(message);
                    RelationInfo* rela_info = rela_svr.GetRelation(publishReq.user_id());
                    for (int i = 0; i < rela_info->friend_count(); i ++) {
                        photo_svr.UpdatePhoto(
                            rela_info->GetFriendUserIdByIndex(i), 
                            publishReq.user_id(), 
                            now, 
                            message.message_id()
                        );
                    }
                    publishRsp.set_ret(SUCCESS);
                }
                else {
                    publishRsp.set_ret(ret);
                }
                break;
            }
            case GET_PHOTO_REQ: {
                ssp::GetPhotoReq getPhotoReq;
                ssp::GetPhotoRsp getPhotoRsp;
                getPhotoReq.ParseFromArray(buffer + 3, 10240);
                ret = user_svr.CheckExist(getPhotoReq.user_id());
                if (ret == USER_EXIST) {
                    PhotoInfo* photo = photo_svr.GetPhoto(getPhotoReq.user_id());
                    if (photo != NULL) {
                        MessageInfo* message = mess_svr.GetMessage(photo->last_publish_message_id()); 
                        if (message != NULL) {
                            getPhotoRsp.mutable_last_message()->set_message_id(message->message_id());
                            getPhotoRsp.mutable_last_message()->set_content(message->content());
                            getPhotoRsp.mutable_last_message()->set_publisher(photo->last_publisher());
                            getPhotoRsp.mutable_last_message()->set_publish_time(photo->last_publish_time());  
                            getPhotoRsp.set_ret(SUCCESS);
                        }
                        else {
                            getPhotoRsp.set_ret(MESSAGE_NOT_EXIST);
                        }
                    } 
                    else {
                        getPhotoRsp.set_ret(PHOTO_NOT_EXIST);
                    }    
                }
                else {
                    getPhotoRsp.set_ret(ret);
                }
                break;
            }
            case GET_MESSAGE_LIST_REQ: {
                ssp::GetMessageReq getMessageReq;
                ssp::GetMessageRsp getMessageRsp;
                getMessageReq.ParseFromArray(buffer + 3, 10240);
                ret = user_svr.CheckExist(getMessageReq.user_id());
                if (ret == USER_EXIST) {
                    user_svr.UpdateUserFreshTime(getMessageReq.user_id(), now);
                    getMessageRsp.set_ret(SUCCESS);
                }
                else {
                    getMessageRsp.set_ret(ret);
                }
                break;
            }
            case DEL_FRIEND_REQ: {
                ssp::DelFriendReq delFriendReq;
                ssp::DelFriendRsp delFriendRsp;
                delFriendReq.ParseFromArray(buffer + 3, 10240);
                ret = user_svr.CheckExist(delFriendReq.user_id());
                if (ret == USER_EXIST) {
                    ret = user_svr.CheckExist(delFriendReq.other_id());
                    if (ret == USER_EXIST) {
                        ret = rela_svr.DeleteFriend(delFriendReq.user_id(), delFriendReq.other_id());
                        delFriendRsp.set_ret(ret);
                    }
                    else {
                        delFriendRsp.set_ret(ret);
                    }
                }
                else {
                    delFriendRsp.set_ret(ret);
                }
                break;
            }
            case DEL_BLACK_REQ: {
                ssp::DelBlackReq delBlackReq;
                ssp::DelBlackRsp delBlackRsp;
                delBlackReq.ParseFromArray(buffer + 3, 10240);
                ret = user_svr.CheckExist(delBlackReq.user_id());
                if (ret == USER_EXIST) {
                    ret = user_svr.CheckExist(delBlackReq.other_id());
                    if (ret == USER_EXIST) {
                        ret = rela_svr.DeleteBlack(delBlackReq.user_id(), delBlackReq.other_id());
                        delBlackRsp.set_ret(ret);
                    }
                    else {
                        delBlackRsp.set_ret(ret);
                    }
                }
                else {
                    delBlackRsp.set_ret(ret);
                }
                break;
            }
            case LOGOUT_REQ: {
                ssp::LogoutReq logoutReq;
                ssp::LogoutRsp logoutRsp;
                logoutReq.ParseFromArray(buffer + 3, 10240);
                ret = user_svr.CheckExist(logoutReq.user_id());
                if (ret == USER_EXIST) {
                    user_svr.UserLogout(logoutReq.user_id(), now);
                    logoutRsp.set_ret(ret);
                }
                else {
                    logoutRsp.set_ret(ret);
                }
                break;
            }
            default:
                break;
        }
        user_svr.Proc();
        rela_svr.Proc();
        mess_svr.Proc();
        photo_svr.Proc();
        mess_type = -1;

        usleep(5000);
        break;
    }
    
    user_svr.Shutdown();
    rela_svr.Shutdown();
    mess_svr.Shutdown();
    photo_svr.Shutdown();
    return 0;
}