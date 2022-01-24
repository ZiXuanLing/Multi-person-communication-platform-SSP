#ifndef __USER_MANAGER_H__

#define __USER_MANAGER_H__

#include "UserInfo.h"

#include "DbManager.h"

class UserManager {
private:
    UserInfo users[10240];
    GETSETVAR(int, user_count)
    DbManager db_svr;
private:
    MYSQL* conn;
    MYSQL_RES* result;
    MYSQL_ROW row;
private:
    int Init();
public:
    void Start();
    void Proc();
    void Shutdown();
    void Restart();
public:
    int ShowAll();
    UserInfo* GetUser(int user_id);
    int CheckExist(int user_id);
    int CreateUser(int user_id, char* user_name, char* pswd);
    int DeleteUser(int user_id);
    int UpdateUserLoginTime(int user_id, char* pswd, int time_now);
};

#endif // !__USER_MANAGER_H__