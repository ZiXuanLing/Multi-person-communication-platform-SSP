#include "DbManager.h"

#include <stdio.h>

using namespace std;

int main()
{
    DbManager db_svr;
    db_svr.initDb("127.0.0.1", "test", "123456", "ssp");
    // db_svr.execSql("select * from dbtest;");
    // db_svr.execSql("insert into dbtest values(2, 'kkb', 344, 'lhiuhao');");
    // db_svr.execSql("select * from dbtest;");
    // db_svr.execSql("delete from dbtest where id=2;");
    db_svr.execSql("select * from dbtest;");
    return 0;
}