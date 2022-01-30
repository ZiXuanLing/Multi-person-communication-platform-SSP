#include "UserManager.h"

#include "common/ret_value.h"

#include <stdio.h>

int str2int(char *str)
{
    int ret = 0;
    int len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        ret = ret * 10 + (str[i] - '0');
    }
    return ret;
}

int UserManager::Init()
{
    set_user_count(0);
    conn = mysql_init(NULL);
    if (conn == NULL)
    {
        return DB_CONN_INIT_FAIL;
    }
    conn = mysql_real_connect(conn,
                              "127.0.0.1",
                              "test",
                              "123456",
                              "ssp",
                              0,
                              NULL,
                              0);
    if (conn == NULL)
    {
        return DB_CONN_CONNECT_FAIL;
    }
    if (mysql_query(conn, "select * from db_user;"))
    {
        printf("select fail: %s \n", mysql_error(conn));
        return DB_QUERY_FAIL;
    }
    else
    {
        result = mysql_use_result(conn);
        if (result)
        {
            int num_fields_1 = mysql_num_fields(result); // row
            int num_fields_2 = mysql_field_count(conn);  // The number of users
            // printf("num_fields_1 %d, num_fields_2 %d\n", num_fields_1, num_fields_2);
            for (int i = 0; i < num_fields_2; i++)
            { // Read data by row
                row = mysql_fetch_row(result);
                if (row == NULL)
                {
                    break;
                }
                for (int j = 0; j < num_fields_1; j++)
                {
                    printf("%s\t", row[j]);
                }
                ssp::UserInfoBase pb_user;
                pb_user.ParseFromArray(row[1], sizeof(row[1]));
                // pb_user.user_id();
                users[i].set_user_id(pb_user.user_id());
                // users[i].set_user_name(const_cast<char *>(pb_user.user_name().data()));
                // users[i].set_nick_name(const_cast<char *>(pb_user.nick_name().data()));
                users[i].set_db_flag(FLAG_INIT); /* 0 indicates no deletion */
                printf("\n");
            }
            set_user_count(num_fields_2);
        }
        mysql_free_result(result);
    }
    printf("UserManager Init\n");
    return SUCCESS;
}

int UserManager::Start()
{
    set_cur_user_id(10001);
    int ret = Init(); /* Initialize exactly at the beginning, such as connecting to the database to load data and so on */
    if (ret != SUCCESS)
    {
        printf("UserManager Start Fail\n");
        return -1;
    }
    printf("UserManager Start\n");
    return SUCCESS;
}

int UserManager::Proc()
{

    printf("UserManager Running\n");
    printf("User Online %d\n", 0);       // todo  Perform performance analysis
    printf("User Online time: %d\n", 0); // todo
    printf("User reg num: %d\n", reg_num());     // todo

    return SUCCESS;
}

int UserManager::Shutdown()
{
    /* Back to the write operation */
    for (int i = 0; i < user_count_; i++)
    {
        /* 1 delete */
        if (users[i].db_flag() == FLAG_DELETE)
        {
            string deleSql = "delete from db_user where id=" + users[i].user_id() + ';';
            if (mysql_query(conn, deleSql.c_str()))
            {
                printf("delete fail: %s \n", mysql_error(conn));
            }
        }
        /* 2 update */
        if (users[i].db_flag() == FLAG_UPDATE)
        {
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
            char user_id[256];
            sprintf(user_id, "%d", users[i].user_id());
            string updateSql = "update tb_user set user_info='";
            updateSql += data;
            updateSql += "' where id=";
            updateSql += user_id;
            updateSql += ";";
            if (mysql_query(conn, updateSql.c_str()))
            {
                printf("update fail: %s \n", mysql_error(conn));
            }
        }
    }
    printf("UserManager Shutdown\n");
    return SUCCESS;
}

int UserManager::Restart()
{
    printf("User Manager Restart\n");
    return SUCCESS;
}

int UserManager::ShowAll()
{
    printf("=====================================\n");
    for (int i = 0; i < user_count_; i++)
    {
        printf("| %d | %s | %s | \n", users[i].user_id(), users[i].user_name(), users[i].password());
    }
    printf("=====================================\n");
    return 0;
}

UserInfo *UserManager::GetUser(int user_id)
{
    for (int i = 0; i < user_count_; i++)
    {
        if (users[i].user_id() == user_id)
        {
            return &users[i];
        }
    }
    return NULL;
}


/**
 * @brief Create user information on the server
 * 
 * @param user_name username
 * @param pswd password
 * @param from Registered source
 * @param time_now Now the server time
 * @return int 
 */
int UserManager::CreateUser(const char *user_name, const char *pswd, int from, int time_now)
{
    for (int i = 0; i < user_count_; i++)
    {
        if (strcmp(users[i].user_name(), user_name) == 0)
        { // Delete the account name
            return USER_EXIST;
        }
    }
    // int cur_user_id = cur_user_id; // GetVarUserId(); // todo get user_id from tb_var str='user_id'
    
    if (user_count_ < 10239)
    {
        users[user_count_].set_user_id(cur_user_id());
        users[user_count_].set_user_name(const_cast<char *>(user_name));
        users[user_count_].set_password(const_cast<char *>(pswd));
        users[user_count_].set_from(from);
        users[user_count_].set_reg_time(time_now);
        user_count_++;
        // return SUCCESS;
    }
    set_cur_user_id(cur_user_id() + 1);
    set_reg_num(reg_num() + 1); // Number of registrations plus one
    return 0;
}

int UserManager::DeleteUser(int user_id)
{
    for (int i = 0; i < user_count_; i++)
    {
        if (users[i].user_id() == user_id)
        {
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
int UserManager::UpdateUserLoginTime(int user_id, int time_now)
{
    UserInfo *user = GetUser(user_id);
    if (user == NULL)
    {
        printf("USER_NOT_EXIST %d\n", user_id);
        return USER_NOT_EXIST;
    }
    user->set_login_time(time_now);
    return SUCCESS;
}

/**
 * @brief Check whether the user exists
 *
 * @param user_id
 * @return int
 */
int UserManager::CheckExist(int user_id)
{
    UserInfo *user = GetUser(user_id);
    if (user == NULL)
    {
        printf("USER_NOT_EXIST %d\n", user_id);
        return USER_NOT_EXIST;
    }
    return USER_EXIST;
}

/**
 * @brief User login verification
 *
 * @param user_name username for user
 * @param user_pswd password for user
 * @return int
 */
int UserManager::LoginCheck(const char *user_name, const char *password)
{
    for (int i = 0; i < user_count_; i++)
    {
        if (strcmp(users[i].user_name(), user_name) == 0)
        {
            if (strcmp(users[i].password(), password) == 0)
            {
                return SUCCESS;
            }
            else
            {
                return WRONG_PASSWORD;
            }
        }
    }
    return USER_NOT_EXIST;
}

/**
 * @brief Obtain the user ID from the user account
 *
 * @param user_name
 * @return int
 */
int UserManager::GetUserIdByUserName(const char *user_name)
{
    for (int i = 0; i < user_count_; i++)
    {
        if (strcmp(users[i].user_name(), user_name) == 0)
        {
            return users[i].user_id();
        }
    }
    return USER_NOT_EXIST;
}

/**
 * @brief Exit the user state handler
 *
 * @param user_id
 * @return int
 */
int UserManager::UserLogout(int user_id, int now)
{
    UserInfo *user = GetUser(user_id);
    if (user == NULL)
    {
        return USER_NOT_EXIST;
    }
    user->set_logout_time(now);
    return SUCCESS;
}

/**
 * @brief Refresh User refresh time
 * 
 * @param user_id 
 * @param now 
 * @return int 
 */
int UserManager::UpdateUserFreshTime(int user_id, int time_now)
{
    UserInfo *user = GetUser(user_id);
    if (user == NULL)
    {
        return USER_NOT_EXIST;
    }
    user->set_fresh_time(time_now);
    return SUCCESS;
}