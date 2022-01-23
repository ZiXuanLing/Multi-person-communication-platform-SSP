#include "user_info.h"

/**
 * @brief Create a user
 * @param user User structure
 * @param user_name Registered user name
 * @param now Registration time
 * @param from Registered channels
 * @return success: 0 failure: 0
*/
int user_create(UserInfo *user, char *user_name, int now, int from) {
    user->user_id = 0;
    strncpy(user->user_name, user_name, 256);

    strcpy(user->nick_name, user->user_name);

    user->reg_time = now;
    user->from = from;
    user->login_time = 0;
    user->last_login_time = 0;
    user->fresh_time = 0;

    int ret = createUser(user);
    if (ret == 0) {
        // save DB
    }
    return 0;
}
/**
 * @brief The user login
 * @param user User structure
 * @param now The login time
 * @return ret
*/
int user_login(UserInfo *user, int now) {
    // get userInfo from DB
    int ret = getUser(user, user->user_id);
    if (ret == 0) {
        user->login_time = now;
    }
    return ret;
}
/**
 * @brief The user logout
 * @param user User structure
 * @param now The login time
 * @return ret
*/
int user_logout(UserInfo *user, int now) {
    int ret = LogoutUser(user);
    if (ret == 0) {
        // DB save 
    }
    return ret; // ret < 0
}
/**
 * @brief The user delete
 * @param user User structure
 * @param now The login time
 * @return ret
*/
int user_delete(UserInfo *user) {
    int ret;
    ret = deleteUser(user);
    if (ret == 0) {
        // DB save 
    }
    return ret;
}