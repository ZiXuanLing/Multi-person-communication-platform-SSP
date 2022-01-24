#ifndef __MESSAGE_INFO_H__

#define __MESSAGE_INFO_H__

#include "common/macro.h"

#include "proto/MessageInfoBase.pb.h"

#include <string.h>

class MessageInfo {
    GETSETVAR(int, user_id)
    GETSETVAR(int, message_id)
    GETSETSTR(10240, content)
};

#endif // !__MESSAGE_INFO_H__