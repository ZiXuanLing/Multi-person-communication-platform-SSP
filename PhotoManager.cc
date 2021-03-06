#include "PhotoManager.h"

void PhotoManager::Start()
{
    // printf("PhotoManager Start\n");
}

void PhotoManager::Proc()
{
    // printf("PhotoManager Proc\n");
}

void PhotoManager::Shutdown()
{
    // printf("PhotoManager Shutdown\n");
}

void PhotoManager::Restart()
{
    // printf("PhotoManager Restart\n");
}

PhotoInfo *PhotoManager::GetPhoto(int user_id)
{
    for (int i = 0; i < photo_count_; i++)
    {
        if (photos_[i].user_id() == user_id)
        {
            return &photos_[i];
        }
    }
    return NULL;
}

int PhotoManager::UpdatePhoto(int user_id, int publisher_id, int publish_time, int publish_message_id)
{
    PhotoInfo *photo = GetPhoto(user_id);
    if (photo == NULL)
    {
        return PHOTO_NOT_EXIST;
    }
    photo->set_user_id(user_id); // not need
    photo->set_last_publisher(publisher_id);
    photo->set_last_publish_time(publish_time);
    photo->set_last_publish_message_id(publish_message_id);
    return SUCCESS;
}