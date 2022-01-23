#include "UserManager.h"

#include <stdio.h>
#include <unistd.h>

UserManager user_svr;

int main() {
    user_svr.Start();

    while (1) {
        user_svr.Proc();
        sleep(1);
    }
    user_svr.Shutdown();
    return 0;
}