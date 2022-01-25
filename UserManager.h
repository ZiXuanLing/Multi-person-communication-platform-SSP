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
    int Start();
    int Proc();
    int Shutdown();
    int Restart();
public:
    int ShowAll();
    UserInfo* GetUser(int user_id);
    int CheckExist(int user_id);
    int CreateUser(const char* user_name, const char* pswd, int from);
    int DeleteUser(int user_id);
    int LoginCheck(const char* user_name, const char* user_pswd);
    int UpdateUserLoginTime(int user_id, int time_now);
    int GetUserIdByUserName(const char* user_name);
};

#endif // !__USER_MANAGER_H__