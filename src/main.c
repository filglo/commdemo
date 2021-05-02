#include <time.h>
#include <stdlib.h>
#include "messenger.h"

int main()
{
    int fdRecv = -1;
    int fdSend = -1;
    int progId;
    int ret;
    srand(time(NULL));
    progId = initRecv(&fdRecv);
    ret = eventLoop(&fdSend, &fdRecv, progId);
    atExit(fdSend, fdRecv);

    return ret;
}
