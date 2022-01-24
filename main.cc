#include "UserManager.h"
#include "RelationManager.h"
#include "MessageManager.h"
#include "PhotoManager.h"

#include <stdio.h>
#include <unistd.h>

UserManager user_svr;
RelationManager relation_svr;
MessageManager message_svr;
PhotoManager photo_svr;

int main() {
    user_svr.Start();
    relation_svr.Start();
    message_svr.Start();
    photo_svr.Start();
    while (1) {
        int type = 0; //
        switch (type) {
            case 1:
                user_svr.Proc();
                break;
            case 2:
                relation_svr.Proc();
                break;
            case 3:
                message_svr.Proc();
                break;
            case 4:
                photo_svr.Proc();
                break;
        }
        usleep(5000);
        break;
    }
    user_svr.Shutdown();
    relation_svr.Shutdown();
    message_svr.Shutdown();
    photo_svr.Shutdown();
    return 0;
}