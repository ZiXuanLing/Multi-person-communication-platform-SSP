#ifndef __PHOTO_INFO_H_

#define __PHOTO_INFO_H_

#include "common/macro.h"

#include "proto/PhotoInfoBase.pb.h"

using namespace ssp;

class PhotoInfo
{
public:
    void FromPb(ssp::PhotoInfoBase &photo);
    void ToPb(ssp::PhotoInfoBase &photo);

private:
    GETSETVAR(int, user_id);
    GETSETVAR(int, last_publisher);
    GETSETVAR(int, last_publish_time);
    GETSETVAR(int, last_publish_message_id);
};

#endif // !__PHOTO_INFO_H_