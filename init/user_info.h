#ifndef __USER_INFO_H__
#define __USER_INFO_H__

/**
 * User management module, including login, logout, registration, exit four functions
*/

#include "ret_define.h"

#include <stdio.h>
#include <string.h>
/**
 * Define a structure that contains user information
*/
typedef struct _UserInfo_ {
    int user_id;    // Unique identifier of the user
    char user_name[256];    
    char nick_name[256];
    int reg_time;
    int from;// wx1,qq2,tel3 Registered channels
    int login_time;
    int last_login_time;
    int fresh_time;
    // password
} UserInfo;

int user_create(UserInfo *user, char *user_name, int now, int from);
int user_login(UserInfo *user, int now);
int user_logout(UserInfo *user, int now);
int user_delete(UserInfo *user);

#endif // !__USER_INFO_H__