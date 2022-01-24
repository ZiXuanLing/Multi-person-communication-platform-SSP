#include "UserManager.h"

#include "common/ret_value.h"

#include <stdio.h>

int str2int(char* str) {
    int ret = 0;
    int len = strlen(str);
    for (int i = 0; i < len; i ++) {
        ret = ret * 10 + (str[i] - '0');
    }
    return ret;
}

int UserManager::Init() {
    set_user_count(0);
    conn = mysql_init(NULL);   
    if (conn == NULL) {
        return DB_CONN_INIT_FAIL;
    }
    conn = mysql_real_connect(conn, 
        "127.0.0.1", 
        "test", 
        "123456", 
        "ssp",
        0,
        NULL,
        0
    );
    if (conn == NULL) {
        return DB_CONN_CONNECT_FAIL;
    } 
    if (mysql_query(conn, "select * from db_user;")) {
        printf("select fail: %s \n", mysql_error(conn));
        return DB_QUERY_FAIL;
    }
    else {
        result = mysql_use_result(conn);
        if (result) {
            int num_fields_1 = mysql_num_fields(result); // row
            int num_fields_2 = mysql_field_count(conn); // The number of users
            // printf("num_fields_1 %d, num_fields_2 %d\n", num_fields_1, num_fields_2);
            for (int i = 0; i < num_fields_2; i ++) {   // Read data by row
                row = mysql_fetch_row(result);
                if (row == NULL) {
                    break;
                }
                for (int j = 0; j < num_fields_1; j ++) {
                    printf("%s\t", row[j]);
                }
                // users[i].set_user_id(str2int(row[0]));
                // users[i].set_user_name()
                ssp::UserInfoBase pb_user;
                pb_user.CopyFrom(row[1]);   /* deserialization */ 
                // pb_user.user_id();
                users[i].set_user_id(pb_user.user_id());
                users[i].set_user_name(pb_user.user_name());
                users[i].set_nick_name(pb_user.nick_name());
                users[i].set_db_flag(0);    /* 0 indicates no deletion */
                printf("\n");
            }
            set_user_count(num_fields_2);
        }
        mysql_free_result(result);
    }
    printf("User Manager Init\n");
}

void UserManager::Start() {
    int ret = Init(); /* Initialize exactly at the beginning, such as connecting to the database to load data and so on */
    if (ret != SUCCESS) {
        printf("User Manager Start Fail\n");
        return;
    }
    printf("User Manager Start\n");
}

void UserManager::Proc() {
    printf("User Manager Proc\n");
}

void UserManager::Shutdown() {
    /* Back to the write operation */
    for (int i = 0; i < user_count_; i ++) {
        /* 1 delete */
        if (users[i].db_flag() == FLAG_DELETE) {
            string deleSql = "delete from db_user where id=" + users[i].user_id() + ';';
            if (mysql_query(conn, deleSql.c_str())) {
                printf("delete fail: %s \n", mysql_error(conn));
            }
        }
        /* 2 update */
        if (users[i].db_flag() == FLAG_UPDATE) {
            ssp::UserInfoBase pb_user;
            pb_user.set_user_id(users[i].user_id());
            pb_user.set_user_name(users[i].user_name());
            pb_user.set_nick_name(users[i].nick_name());
            pb_user.set_reg_time(users[i].reg_time());
            pb_user.set_from(users[i].from());
            pb_user.set_login_time(users[i].login_time());
            pb_user.set_last_login_time(users[i].last_login_time());
            pb_user.set_fresh_time(users[i].fresh_time());
            // password
            // db_flag
            char data[10240];
            pb_user.SerializeToArray(data, 10240); /* Write back to the database */
            string updateSql = "update tb_user set str='" + string(data) + "' where id=" + pb_user.user_id() + ";";
            if (mysql_query(conn, updateSql.c_str())) {
                printf("update fail: %s \n", mysql_error(conn));
            }
        }
    }
    printf("User Manager Shutdown\n");
}

void UserManager::Restart() {
    printf("User Manager Restart\n");
}

int UserManager::ShowAll() {
    printf("=====================================\n");
    for (int i = 0; i < user_count_; i ++) {
        printf("| %d | %s | %s | \n", users[i].user_id(), users[i].user_name(), users[i].password());
    }
    printf("=====================================\n");
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
            users[i].set_db_flag(FLAG_DELETE);
            return USER_EXIST;
        }
    }
    return OK;
}

/**
 * @brief Update the login time
 * 
 * @param user_id 
 * @param time_now The current time
 * @return int 
 */
int UserManager::UpdateUserLoginTime(int user_id, char* pswd, int time_now) {
    UserInfo* user = GetUser(user_id);
    if (user == NULL) {
        printf("USER_NOT_EXIST %d\n", user_id);
        return USER_NOT_EXIST;
    }
    if (strcmp(user->password(), pswd) == 0) {
        user->set_login_time(time_now);
    }
    else {
        printf("WRONG_PASSWORD %d Acturel: %s Expect: %s\n", user_id, pswd, user->password());
        return WRONG_PASSWORD;
    }
    return SUCCESS;
}

/**
 * @brief Check whether the user exists
 * 
 * @param user_id 
 * @return int 
 */
int UserManager::CheckExist(int user_id) {
    UserInfo* user = GetUser(user_id);
    if (user == NULL) {
        printf("USER_NOT_EXIST %d\n", user_id);
        return USER_NOT_EXIST;
    }
    return USER_EXIST;
}