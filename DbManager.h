#ifndef __DBMANAGER_H__

#define __DBMANAGER_H__

/* The MySQL dependency packages need to be installed */
#include "mysql/mysql.h"

#include "proto/user_info_base.pb.h"

#include "UserInfo.h"

#include <string>
#include <iostream>

using namespace std;

class DbManager {
private:
    MYSQL* conn;
    MYSQL_RES* result;
    MYSQL_ROW row;
public:
    int initDb(string host, string user, string pwsd, string db_name);
    int execSql(string sql); /* execute sql statement */
    int GetUserInfo(UserInfo& users);
};

#endif // !__DBMANAGER_H__