#include "user_info.h"
#include "friend_info.h"

#define MAX_USER 1024
#define MAX_FRIEND 1048576

UserInfo *_UserFromDB_; // Stores an array of all user information structures
FriendInfo *_FriendFromDB_; // Stores an array of all relational structures
/**
 * @brief Initialize the structure array
*/
int startup() {
    // get from DB
    return 0;
}
/**
 * @brief Close the structure array
*/
int shutdown() {
    int user_size = sizeof(_UserFromDB_) / sizeof(UserInfo);
    for (int i = 0; i < user_size; i ++) {
        if (_UserFromDB_[i]->user_id != USER_NEED_DELETE) {
            // write DB
        }
    }
    int friend_size = sizeof(_FriendFromDB_) / sizeof(FriendInfo);
    for (int i = 0; i < friend_size; i ++) {
        if (_FriendFromDB_[i]->user_id_1 != FRIEND_NEED_DELETE && _FriendFromDB_[i]->user_id_1 != BLACK_NEED_DELETE) {
            // write DB
        }
    }
    return 0;   
}
/**
 * @brief Obtaining user ID
 * @param user 
 * @return Return subscript if user exists otherwise USER_NOT_EXIST
*/
int getUser(UserInfo *user) {
    // if user is exist return USE_EXIST
    int len = sizeof(_UserFromDB_) / sizeof(UserInfo);

    for (int i = 0; i < len; i ++) {
        if (_UserFromDB_[i]->user_id == user->user_id) {
            return i;
        }
    }
    // 
    return USER_NOT_EXIST;
    //

}
/**
 * @brief Create a user
 * @param user User information structure
 * @return SUCCESS is returned on SUCCESS, USER_TO_MUCH is returned when the user array reaches the upper limit, 
 *         and USE_EXIST is returned on failure
*/
int createUser(UserInfo *user) {
    int ret = getUser(user->user_id);
    if (ret == USER_NOT_EXIST) {
        int len = sizeof(_UserFromDB_) / sizeof(UserInfo);
        if (len >= MAX_USER) {
            return USER_TO_MUCH;
        }
        _UserFromDB_[len]->user_id = user->user_id;
        strcpy(_UserFromDB_[len]->user_name, user->user_name);
        // ...
        return SUCCESS;
    }
    else {
        return USE_EXIST;
    }
}
/**
 * @brief Delete a user
 * @param user User information structure
 * @return 
*/
int deleteUser(UserInfo *user) {
    int index = getUser(user->user_id);
    if (index == USE_EXIST) {
        _UserFromDB_[index]->user_id = USER_NEED_DELETE; // In this case, the USER_NEED_DELETE state is marked for the time being
    }
    else {
        return USER_NOT_EXIST;
    }
}
/**
 * @brief Determine whether two users are friends
 * @param self_id Customer user_id
 * @param other_id The user_id of the customer's friend
 * @return Return IS_FRIEND if it is a friend, NOT_FRIEND otherwise
*/
int getFriendInfo(int self_id, int other_id, int type) {
    if (type != FRIEND_TYPE || type != BLACK_TYPE) {
        return TYPE_NOT_DEFINE;
    }
    int friend_size = sizeof(_FriendFromDB_) / sizeof(FriendInfo);
    for (int i = 0; i < friend_size; i ++) {
        if (_FriendFromDB_[i]->user_id_1 == self_id && _FriendFromDB_[i]->user_id_2 == other_id \
            && _FriendFromDB_[i]->type == type) {
            return IS_FRIEND;
        }
    }
    if (type == FRIEND_TYPE) {
        return NOT_FRIEND;
    }
    if (type == BLACK_TYPE) {
        return NOT_BLACK;
    }
    return TYPE_NOT_DEFINE;
}
/**
 * @brief Add a pair of relational constructs
 * @param self_id Customer user_id
 * @param other_id The user_id of the customer's friend
*/
int addFriendInfo(int self_id, int other_id) {
    int friend_index = getFriendInfo(self_id, other_id, FRIEND_TYPE);
    if (friend_index != NOT_FRIEND) {   // They are not good friends
        int size = sizeof(_FriendFromDB_) / sizeof(FriendInfo);
        if (size >= MAX_FRIEND - 1) {
            return FRIEND_TO_MUCH;
        }
        _FriendFromDB_[friend_index]->user_id_1 = self_id;
        _FriendFromDB_[friend_index]->user_id_2 = other_id;
        _FriendFromDB_[friend_index]->type = FRIEND_TYPE;
    }
    return IS_FRIEND;
}
/**
 * @brief Delete a pair of relational constructs
 * @param self_id Customer user_id
 * @param other_id The user_id of the customer's friend
*/
int deleteFriendInfo(int self_id, int other_id) {
    int friend_index = getFriendInfo(self_id, other_id, FRIEND_TYPE);
    if (friend_index == NOT_FRIEND) {   // They are not good friends
        return NOT_FRIEND;
    }
    _FriendFromDB_[friend_index]->user_id_1 = FRIEND_NEED_DELETE;
    return SUCCESS;
}
int getBlackInfo(int self_id, int other_id, int type) {
    return getFriendInfo(self_id, other_id, type);
}
/**
 * @brief Adds a relationship from the relationship structure array
 * @param self_id Customer user_id
 * @param other_id The user_id of the customer's friend
 * @return
*/
int addBlack(int self_id, int other_id) {
    int black_index = getBlackInfo(self_id, other_id, BLACK_TYPE);
    if (black_index == NOT_BLACK) {
        int size = sizeof(_FriendFromDB_) / sizeof(FriendInfo);
        if (size >= MAX_FRIEND - 1) {
            return BLACK_TO_MUCH;
        }
        _FriendFromDB_[black_index]->user_id_1 = self_id;
        _FriendFromDB_[black_index]->user_id_2 = other_id;
        _FriendFromDB_[black_index]->type = BLACK_TYPE;
    }
    return IS_BLACK;
}
/**
 * @brief Deletes a relationship from the relationship structure array
 * @param self_id Customer user_id
 * @param other_id The user_id of the customer's friend
 * @return
*/
int deleteBlack(int self_id, int other_id) {
    int black_index = getBlackInfo(self_id, other_id, BLACK_TYPE);
    if (black_index == NOT_BLACK) {   // They are not good friends
        return NOT_BLACK;
    }
    _FriendFromDB_[black_index]->user_id_1 = BLACK_NEED_DELETE;
    return SUCCESS;
}
int main() {
    startup();// get UserInfo from DB; get FriendInfo from DB
    while (1) {
        // tcp listen
    }
    shutdown(); // clwrite UserInfo to DB;write FriendInfo to DB
}