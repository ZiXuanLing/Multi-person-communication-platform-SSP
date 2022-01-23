#ifndef __DBMANAGER_H__

#define __DBMANAGER_H__

/* The MySQL dependency packages need to be installed */
#include "mysql/mysql.h"

#include <string>

class DbManager {
private:
    MYSQL* conn;
    MYSQL_RES* result;
    MYSQL_ROW row;
public:
    int initDb(std::string host, std::string user, std::string pwsd, std::string db_name);
    int execSql(std::string sql); /* execute sql statement */
};

#endif // !__DBMANAGER_H__