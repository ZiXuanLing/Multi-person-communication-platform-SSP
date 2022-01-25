#include "RelationManager.h"

#include "common/ret_value.h"

#include <stdio.h>

void RelationManager::Start() {
    printf("Relation Manager Start\n");
}

void RelationManager::Proc() {
    printf("Relation Manager Proc\n");
}

void RelationManager::Shutdown() {
    printf("Relation Manager Shutdown\n");
}

void RelationManager::Restart() {
    printf("Relation Manager Restart\n");
}

RelationInfo* RelationManager::GetRelation(int user_id) {
    for (int i = 0; i < relation_count_; i ++) {
        if (relations_[i].user_id() == user_id) {
            return &relations_[i];
        }
    }
    return NULL;
}

int RelationManager::AddFriend(int user_id, int other_id) {
    RelationInfo* friend_list = GetRelation(user_id);
    if (friend_list == NULL) {
        return RELATION_NOT_FOUND;
    }
    int ret = friend_list->AddFriend(other_id);
    return ret;
}

int RelationManager::DeleteFriend(int user_id, int other_id) {
    RelationInfo* friend_list = GetRelation(user_id);
    if (friend_list == NULL) {
        return RELATION_NOT_FOUND;
    }
    int ret = friend_list->DeleteFriend(other_id);
    return ret;
}

int RelationManager::AddBlack(int user_id, int other_id) {
    RelationInfo* black_list = GetRelation(user_id);
    if (black_list == NULL) {
        return RELATION_NOT_FOUND;
    }
    int ret = black_list->AddBlack(other_id);
    return ret;
}

int RelationManager::DeleteBlack(int user_id, int other_id) {
    RelationInfo* black_list = GetRelation(user_id);
    if (black_list == NULL) {
        return RELATION_NOT_FOUND;
    }
    int ret = black_list->DeleteBlack(other_id);
    return ret;
}
