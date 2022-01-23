#ifndef __USERMANAGER_H__

#define __USERMANAGER_H__

#include "UserInfo.h"


class UserManager {
private:
    UserInfo users[10240];
    GETSETVAR(int, user_count)
public:
    void Start();
    void Proc();
    void Shutdown();
    void Restart();
public:
    UserInfo* GetUser(int user_id);
    int CreateUser(int user_id, char* user_name, char* pswd);
    int DeleteUser(int user_id);
};

#endif // !__USERMANAGER_H__