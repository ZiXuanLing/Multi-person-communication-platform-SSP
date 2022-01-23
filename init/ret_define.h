#ifndef __ERR_DEFINE_H__
#define __ERR_DEFINE_H__

/**
 * Common macros are defined
 * interval isolation is performed to quickly find errors later
*/

#define SUCCESS 0

#define USE_EXIST -1
#define USER_NOT_EXIST -2   // User does not exist
#define ALREADY_FRIEND -3
#define ALREADY_BLACK -4
#define TYPE_NOT_DEFINE -5

#define USER_NEED_DELETE -100

#define FRIEND_TYPE 0
#define IS_FRIEND -200
#define NOT_FRIEND -201
#define FRIEND_NEED_DELETE -202
#define FRIEND_TO_MUCH -203

#define BLACK_TYPE 1
#define IS_BLACK -300
#define NOT_BLACK -301
#define BLACK_NEED_DELETE -302
#define BLACK_TO_MUCH -303

#endif // !__ERR_DEFINE_H__