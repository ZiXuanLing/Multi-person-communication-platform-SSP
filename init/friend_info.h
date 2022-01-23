#ifndef __FRIEND_INFO__
#define __FRIEND_INFO__

/**
 * Relationship management module, including add, delete, shield and shield four functions
*/

#include "ret_define.h"
#include "user_info.h"

#include <stdio.h>
#include <string.h>

/**
 * The buddy relationship structure
*/
typedef struct _FriendInfo_ {
    int user_id_1;
    int user_id_2;
    int type;// 0normal 1blacklist Friend relationship
} FriendInfo;
/**
 * Friend information
*/
typedef struct _UserBrief_ {
    int user_id;
    char user_name[256];
} UserBrief;
/**
 * Friend information structure
*/
typedef struct _FriendList_ {
    int user_id;
    UserBrief *FriendList;   // Normal Friends list
    UserBrief *BlackList; // Block your friend list
} FriendList;

int add_friend(UserInfo *self, UserInfo *other);
int delete_friend(UserInfo *self, UserInfo *other);
int add_black(UserInfo *self, UserInfo *other);
int delete_black(UserInfo *self, UserInfo *other);

#endif // !__FRIEND_INFO__