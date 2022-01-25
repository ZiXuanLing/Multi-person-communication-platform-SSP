#ifndef __ERR_DEFINE_H__
#define __ERR_DEFINE_H__

/**
 * Common macros are defined
 * interval isolation is performed to quickly find errors later
 */

#define OK 0
#define SUCCESS 0
#define ERROR -1

#define USER_EXIST -100
#define USER_NOT_EXIST -101 // User does not exist
#define USER_NEED_DELETE -102
#define WRONG_PASSWORD -103

#define IS_FRIEND -200
#define NOT_FRIEND -201
#define ALREADY_FRIEND -202
#define FRIEND_NEED_DELETE -203
#define FRIEND_TO_MUCH -204
#define RELATION_NOT_FOUND -205

#define IS_BLACK -300
#define NOT_BLACK -301
#define ALREADY_BLACK -302
#define BLACK_NEED_DELETE -303
#define BLACK_TO_MUCH -304

#define MESSAGE_NOT_EXIST -400

#define PHOTO_NOT_EXIST -500

#define DB_CONN_INIT_FAIL -600
#define DB_CONN_CONNECT_FAIL -601
#define DB_QUERY_FAIL -603

#define FLAG_INIT 0
#define FLAG_DELETE 1
#define FLAG_UPDATE 2

#endif // !__ERR_DEFINE_H__