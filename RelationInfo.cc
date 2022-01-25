#include "RelationInfo.h"
#include "common/ret_value.h"

int RelationInfo::CheckFriend(int other_id) {
    for (int i = 0; i < friend_count_; i ++) {
        if (friend_list_[i] == other_id) {
            return ALREADY_FRIEND;
        }
    }
    return NOT_FRIEND;
}

int RelationInfo::AddFriend(int other_id) {
    int check_friend_ret = CheckFriend(other_id);   /* Check if it's a friend */
    if (check_friend_ret == ALREADY_FRIEND) {
        return ALREADY_FRIEND;
    }
    int check_black_ret = CheckBlack(other_id);

    /* Check whether it is a blacklist. If it is a blacklist, the operation of adding friends fails */
    if (check_black_ret == ALREADY_BLACK) { 
        return ALREADY_BLACK;
    }
    if (friend_count_ < 10239) {
        friend_list_[friend_count_] = other_id;
        friend_count_ ++;
    }
    else {
        return FRIEND_TO_MUCH;
    }
    return SUCCESS;
}

int RelationInfo::DeleteFriend(int other_id) {

}

int RelationInfo::CheckBlack(int other_id) {
    for (int i = 0; i < black_count_; i ++) {
        if (black_list_[i] == other_id) {
            return ALREADY_BLACK;
        }
    }
    return NOT_BLACK;
}

int RelationInfo::AddBlack(int other_id) {
    int check_friend_ret = CheckFriend(other_id);   /* Check if it's a friend */
    if (check_friend_ret == ALREADY_FRIEND) {
        return ALREADY_FRIEND;
    }
    int check_black_ret = CheckBlack(other_id);

    /* Check whether it is a blacklist. If it is a blacklist, the operation of adding friends fails */
    if (check_black_ret == ALREADY_BLACK) { 
        return ALREADY_BLACK;
    }
    if (black_count_ < 10239) {
        black_list_[black_count_] = other_id;
        black_count_ ++;
    }
    else {
        return BLACK_TO_MUCH;
    }
    return SUCCESS;
}

int RelationInfo::DeleteBlack(int other_id) {

} 


int RelationInfo::GetFriendUserIdByIndex(int index) {

}