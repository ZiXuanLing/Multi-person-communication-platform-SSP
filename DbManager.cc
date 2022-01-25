#include <stdio.h>

#include "DbManager.h"

#include "common/ret_value.h"

int DbManager::initDb(
    string host, string user, string pwsd, string db_name)
{
    conn = mysql_init(NULL);
    if (conn == NULL)
    {
        return DB_CONN_INIT_FAIL;
    }
    conn = mysql_real_connect(conn,
                              host.c_str(),
                              user.c_str(),
                              pwsd.c_str(),
                              db_name.c_str(),
                              0,
                              NULL,
                              0);
    if (conn == NULL)
    {
        return DB_CONN_CONNECT_FAIL;
    }
    return SUCCESS;
}

int DbManager::execSql(string sql)
{
    if (mysql_query(conn, sql.c_str()))
    {
        printf("query fail: %s \n", mysql_error(conn));
        return DB_QUERY_FAIL;
    }
    else
    {
        result = mysql_use_result(conn);
        if (result)
        {
            int num_fields_1 = mysql_num_fields(result); // row
            int num_fields_2 = mysql_field_count(conn);
            printf("num_fields_1 %d, num_fields_2 %d\n", num_fields_1, num_fields_2);
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
                printf("\n");
            }
        }
        mysql_free_result(result);
    }
    return SUCCESS;
}