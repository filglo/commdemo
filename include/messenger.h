#ifndef MESSENGER_H
#define MESSENGER_H

#define FIFO_PRIMARY "/var/tmp/fifoPrimary"
#define FIFO_SECONDARY "/var/tmp/fifoSecondary"

typedef enum { FALSE=0, TRUE } bool;

int connectSender(const char* pipeName);
int createReceiver(const char* pipeName);
bool pipeStatus(const char* pipeName);
int eventLoop(int* fdSend, int* fdRecv, int progId);
int initRecv(int* fdRecv);
int recvMessage(int fd);
int sendMessage(int fd, const char* msg);
void createRandomMessage(char* msg);
void atExit(int fdSend, int fdRecv);

#endif
