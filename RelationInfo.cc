#include "RelationInfo.h"
#include "common/ret_value.h"

int RelationInfo::CheckFriend(int other_id)
{
    for (int i = 0; i < friend_count_; i++)
    {
        if (friend_list_[i] == other_id)
        {
            return ALREADY_FRIEND;
        }
    }
    return NOT_FRIEND;
}

int RelationInfo::AddFriend(int other_id)
{
    int check_friend_ret = CheckFriend(other_id); /* Check if it's a friend */
    if (check_friend_ret == ALREADY_FRIEND)
    {
        return ALREADY_FRIEND;
    }
    int check_black_ret = CheckBlack(other_id);

    /* Check whether it is a blacklist. If it is a blacklist, the operation of adding friends fails */
    if (check_black_ret == ALREADY_BLACK)
    {
        return ALREADY_BLACK;
    }
    if (friend_count_ < 10239)
    {
        friend_list_[friend_count_] = other_id;
        friend_count_++;
    }
    else
    {
        return FRIEND_TO_MUCH;
    }
    return SUCCESS;
}

int RelationInfo::DeleteFriend(int other_id)
{
    int check_friend_ret = CheckFriend(other_id);
    if (check_friend_ret != ALREADY_FRIEND)
    {
        return NOT_FRIEND;
    }
    int check_black_ret = CheckBlack(other_id);
    if (check_black_ret == ALREADY_BLACK)
    {
        return ALREADY_BLACK;
    }
    for (int i = 0; i < friend_count_; i++)
    {
        if (friend_list_[i] == other_id)
        {
            friend_list_[i] = -1;
        }
    }
    return SUCCESS;
}

int RelationInfo::CheckBlack(int other_id)
{
    for (int i = 0; i < black_count_; i++)
    {
        if (black_list_[i] == other_id)
        {
            return ALREADY_BLACK;
        }
    }
    return NOT_BLACK;
}

int RelationInfo::AddBlack(int other_id)
{
    int check_friend_ret = CheckFriend(other_id); /* Check if it's a friend */
    if (check_friend_ret == ALREADY_FRIEND)
    {
        return ALREADY_FRIEND;
    }
    int check_black_ret = CheckBlack(other_id);

    /* Check whether it is a blacklist. If it is a blacklist, the operation of adding friends fails */
    if (check_black_ret == ALREADY_BLACK)
    {
        return ALREADY_BLACK;
    }
    if (black_count_ < 10239)
    {
        black_list_[black_count_] = other_id;
        black_count_++;
    }
    else
    {
        return BLACK_TO_MUCH;
    }
    return SUCCESS;
}

int RelationInfo::DeleteBlack(int other_id)
{
    int check_friend_ret = CheckFriend(other_id);
    if (check_friend_ret = ALREADY_FRIEND)
    {
        return ALREADY_FRIEND;
    }
    int check_black_ret = CheckBlack(other_id);
    if (check_black_ret != ALREADY_BLACK)
    {
        return NOT_BLACK;
    }
    for (int i = 0; i < black_count_; i++)
    {
        if (black_list_[i] == other_id)
        {
            black_list_[i] = -1;
        }
    }
    return SUCCESS;
}

/**
 * @brief The subscript of the friend_list is used to obtain the user_id of the subscript user
 *
 * @param index
 * @return int
 */
int RelationInfo::GetFriendUserIdByIndex(int index)
{
    if (index < 0 || index >= friend_count_)
    {
        return USER_NOT_EXIST;
    }
    return friend_list_[index];
}

/**
 * @brief Get the user_id of the subscript user by using the subscript of the black_list list
 *
 * @param index
 * @return int
 */
int RelationInfo::GetBlackUserIdByIndex(int index)
{
    if (index < 0 || index >= black_count_)
    {
        return USER_NOT_EXIST;
    }
    return black_list_[index];
}