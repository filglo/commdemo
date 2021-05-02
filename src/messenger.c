#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "messenger.h"
#include "buffer.h"

int connectSender(const char* pipeName)
{
    int fd = open(pipeName, O_WRONLY | O_NDELAY);
    if(fd < 0) { return -1; }
    unlink(pipeName);
    return fd;
}

int createReceiver(const char* pipeName)
{
    int fd;
    if(mkfifo(pipeName, 0666) < 0) { return -1; }
    fd = open(pipeName, O_CREAT | O_RDONLY | O_NDELAY);
    return fd;
}

bool pipeStatus(const char* pipeName)
{
    struct stat st;
    bool pipeExist = FALSE;
    int fd = open(pipeName, O_RDONLY | O_NDELAY);
    if(fd < 0) { return FALSE; }
    pipeExist = fstat(fd, &st) == 0 && S_ISFIFO(st.st_mode);
    close(fd);
    return pipeExist;
}

int eventLoop(int* fdSend, int* fdRecv, int progId)
{
    int j;
    bufferStruct buffer;
    buffer.bufferSize = 0;
    buffer.readId = 0;
    char c;
    const char* helpStr = "g - get messages\n"
    "s - send random message\n"
    "q - quit\n"
    "h - show this message\n";
    // handle sigpipe locally instead of sigpipe handler
    signal(SIGPIPE, SIG_IGN);

    printf("%s", helpStr);
    for(;;)
    {
        c = getchar();
        switch(c)
        {
            case 's':
                createRandomMessage(bufferNextMessage(&buffer));
                break;
            case 'g':
                while(recvMessage(*fdRecv) == 0) {}
                break;
            case 'h':
                printf("%s", helpStr);
                break;
            case 'q':
                return 0;
            default:
                break;
        }
        while ((c = getchar()) != '\n' && c != EOF) { }

        if(*fdSend < 0)
        {
            // connect to pipe on sender end if possible
            *fdSend = connectSender(progId == 1 ? FIFO_SECONDARY : FIFO_PRIMARY);
        }

        while(*fdSend >= 0 && buffer.bufferSize > 0)
        {
            // try to send all messages
            if(sendMessage(*fdSend, getNextMessage(&buffer)) < 0) { return -1; }
            cycleBuffer(&buffer);
        }
    }
    return 0;
}

void atExit(int fdSend, int fdRecv)
{
    if(fdSend >= 0)
    {
        close(fdSend);
    }
    if(fdRecv >= 0)
    {
        close(fdRecv);
    }
    unlink(FIFO_PRIMARY);
    unlink(FIFO_SECONDARY);
}

int initRecv(int* fdRecv)
{
    int progId = -1;
    if(pipeStatus(FIFO_PRIMARY) == FALSE)
    {
        *fdRecv = createReceiver(FIFO_PRIMARY);
        progId = 1;
    }
    else if(pipeStatus(FIFO_SECONDARY) == FALSE)
    {
        *fdRecv = createReceiver(FIFO_SECONDARY);
        progId = 2;
    }
    if(progId < 0 || *fdRecv < 0)
    {
        return -1;
    }
    return progId;
}

int recvMessage(int fd)
{
    char msg[MESSAGE_SIZE];
    int err;
    ssize_t count = read(fd, &msg, MESSAGE_SIZE);
    err = errno;
    if(count > 0)
    {
        printf("Message received: %s\n", msg);
        return 0;
    }
    else if(count == 0 || errno == EAGAIN || errno == EWOULDBLOCK)
    {
        printf("No more messages\n");
        return -1;
    }
    printf("Error during reading: %s\n", strerror(errno));
    return -1;
}

int sendMessage(int fd, const char* msg)
{
    int retcode = write(fd, msg, MESSAGE_SIZE);
    if(retcode < 0)
    {
        printf("Error during writing: %s\n", strerror(errno));
    }
    return retcode;
}

void createRandomMessage(char* msg)
{
    int i;
    for(i = 0; i < MESSAGE_SIZE; i++)
    {
        msg[i] = 32+rand()%93;
    }
}
