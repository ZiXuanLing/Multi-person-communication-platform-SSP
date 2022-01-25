#ifndef __USER_INFO_H__

#define __USER_INFO_H__

#include <string.h>

#include "proto/user_info_base.pb.h"

#include "common/macro.h"

using namespace ssp;

typedef struct UserInfo
{
public:
    void FromPb(ssp::UserInfoBase &user);
    void ToPb(ssp::UserInfoBase &user);

    GETSETVAR(int, user_id)
    GETSETSTR(256, user_name)
    GETSETSTR(256, nick_name)
    GETSETVAR(int, reg_time)
    GETSETVAR(int, from)
    GETSETVAR(int, login_time)
    GETSETVAR(int, last_login_time)
    GETSETVAR(int, fresh_time)
    GETSETSTR(256, password)
    GETSETVAR(int, db_flag)
    GETSETVAR(int, logout_time)
    // password
} UserInfo;

#endif // !__USER_INFO_H__