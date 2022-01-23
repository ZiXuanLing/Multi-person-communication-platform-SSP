#include "UserManager.h"

#include "common/ret_value.h"

#include <stdio.h>

void UserManager::Start() {
    printf("User Manager Start\n");
}

void UserManager::Proc() {
    printf("User Manager Proc\n");
}

void UserManager::Shutdown() {
    printf("User Manager Shutdown\n");
}

void UserManager::Restart() {
    printf("User Manager Restart\n");
}

UserInfo* UserManager::GetUser(int user_id) {
    for (int i = 0; i < user_count_; i ++) {
        if (users[i].user_id() == user_id) {
            return &users[i];
        }
    }
    return NULL;
}

int UserManager::CreateUser(int user_id, char* user_name, char* pswd) {
    if (GetUser(user_id) != NULL) {
        return USER_EXIST;
    }
    else {
        if (user_count_ < 10239) {
            users[user_count_].set_user_id(user_id);
            users[user_count_].set_user_name(user_name);
            users[user_count_].set_password(pswd);
            user_count_ ++;
            return SUCCESS;
        }
    }
    return OK;
}

int UserManager::DeleteUser(int user_id) {
    for (int i = 0; i < user_count_; i ++) {
        if (users[i].user_id() == user_id) {
            printf("USER EXIST %d %s\n", users[i].user_id(), users[i].user_name());
            users[i].set_user_id(-1);
            return USER_EXIST;
        }
    }
    return OK;
}

