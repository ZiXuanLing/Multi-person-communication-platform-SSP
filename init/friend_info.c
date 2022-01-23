#include "friend_info.h"
/**
 * @brief add friend
 * @param self oneself
 * @param other Friends to add
 * @return Whether a friend is added successfully
*/
int add_friend(UserInfo *self, UserInfo *other) {
    int exist_self = getUser(self->user_id);
    int exist_other = getUser(other->user_id);

    if (exist_self != USER_NOT_EXIST && exist_other != USER_NOT_EXIST) {
        // check conflict
        int exist_friend = getFriendInfo(self->user_id, other->user_id);
        if (exist_friend = ALREADY_FRIEND) { // Added friends
            return ALREADY_FRIEND;
        }
        int add_ret = addFriendInfo(self->user_id, other->user_id);
        return add_ret;
    }

    return USER_NOT_EXIST;
}
/**
 * @brief delete friend
 * @param self oneself
 * @param other Friends to add
 * @return Whether a friend is deleted successfully
*/
int delete_friend(UserInfo *self, UserInfo *other) {
    int exist_self = getUser(self->user_id);
    int exist_other = getUser(other->user_id);

    if (exist_self != USER_NOT_EXIST && exist_other != USER_NOT_EXIST) {
        int exist_friend = getFriendInfo(self->user_id, other->user_id);
        if (exist_friend) {
            // delete
            int ret = deleteFriendInfo(self->user_id, other->user_id);
            return ret;
        }
        else {
            return NOT_FRIEND;
        }
    }

    return USER_NOT_EXIST;
}
/**
 * @brief add black
 * @param self oneself
 * @param other black to add
 * @return Whether a black is added successfully
*/
int add_black(UserInfo *self, UserInfo *other) {
    int exist_self = getUser(self->user_id);
    int exist_other = getUser(other->user_id);

    if (exist_self != USER_NOT_EXIST && exist_other != USER_NOT_EXIST) {
        int exist_black = getBlack(self->user_id, other->user_id);
        if (exist_black) {
            return ALREADY_BLACK;
        }
        int ret = addBlack(self->user_id, other->user_id);
        return ret;
    }

    return USER_NOT_EXIST;
}
/**
 * @brief delete black
 * @param self oneself
 * @param other black to delete
 * @return Whether a black is deleted successfully
*/
int delete_black(UserInfo *self, UserInfo *other) {
    int exist_self = getUser(self->user_id);
    int exist_other = getUser(other->user_id);

    if (exist_self != USER_NOT_EXIST && exist_other != USER_NOT_EXIST) {
        int exist_black = getBlack(self->user_id, other->user_id);
        if (exist_black) {
            int ret = deleteBlack(self->user_id, other->user_id);
            return ret;
        }
        else {
            return NOT_BLACK;
        }
    }
    return 0;
}