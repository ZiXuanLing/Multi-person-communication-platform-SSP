#include "UserManager.h"
#include "RelationManager.h"
#include "MessageManager.h"
#include "PhotoManager.h"

#include "common/mess_type.h"

#include "proto/message_define.pb.h"

#include <stdio.h>
#include <unistd.h>

UserManager user_svr;
RelationManager rela_svr;
MessageManager mess_svr;
PhotoManager photo_svr;


int GetMessType() {
    return 101;
}
void Response(ssp::LoginReq) {
    // 
}
int main() {
    user_svr.Start();
    rela_svr.Start();
    mess_svr.Start();
    photo_svr.Start();
    while (1) {
        time_t now = time(NULL);
        ssp::LoginReq lreq;
        ssp::LoginRsp lrsp;
        ssp::AddFriendReq arep;
        ssp::AddFriendRsp arsp;
        ssp::PublishMessageReq preq;
        ssp::PublishMessageRsp prsp;
        switch (GetMessType()) {
            case LOGIN_REQ:
                int ret = user_svr.UpdateUserLoginTime(lreq.user_id(), lreq.password(), now);
                lrsp.set_ret(ret);
                break;
            case ADD_FRIEND_REQ:
                if (user_svr.CheckExist(arep.user_id()) == USER_EXIST && user_svr.CheckExist(arep.other_id()) == USER_EXIST) {
                    int ret = relation_svr.AddFriend(arep.user_id(), arep.other_id());
                    arsp.set_ret(ret);
                }
                else {
                    arsp.set_ret(USER_NOT_EXIST);
                }
                break;
            case PUBLISH_MESSAGE_REQ:
                // write message
                MessageInfo message;
                int current_message_id = GetGlbMessageId();
                message.set_message_id(current_message_id);
                // message.set_user_id(preq.user_id())
                message.set_publisher(preq.user_id());
                message.set_content(preq.content());
                message.set_publish_time(now);
                mess_svr.PushMessage(message);
                // get relation for each friend write photo
                RelationInfo* rela = rela_svr.GetRelationInfo(preq.user_id()); // 此处我觉得是GetRelation
                for (int i = 0; i < rela->friend_count(); i ++) {
                    // write photo
                    photo_svr.UpdatePhoto(rela->user_id(), preq.user_id(), now, current_message_id)
                }
                break;
        }
        Response(lrsp);
        // user_svr.Proc();
        // relation_svr.Proc();
        usleep(5000);
        break;
    }
    
    user_svr.Shutdown();
    relation_svr.Shutdown();
    message_svr.Shutdown();
    photo_svr.Shutdown();
    return 0;
}