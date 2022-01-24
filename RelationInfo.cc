#include "RelationInfo.h"
#include "common/ret_value.h"

int RelationInfo::CheckFriend(int other_id) {

}

int RelationInfo::AddFriend(int other_id) {
    for (int i = 0; i < friend_count_; i ++) {
        if (friend_list_[i] == other_id) {
            return ALREADY_FRIEND;
        }
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

}

int RelationInfo::AddBlack(int other_id) {

}

int RelationInfo::DeleteBlack(int other_id) {

} 
