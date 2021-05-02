#ifndef BUFFER_H
#define BUFFER_H

#define BUFFER_SIZE 5
#define MESSAGE_SIZE 10

typedef struct
{
    int bufferSize;
    int readId;
    char bufferArr[BUFFER_SIZE][MESSAGE_SIZE];
} bufferStruct;

// returns pointer to buffer for next message
char* bufferNextMessage(bufferStruct* buffer);
// returns next message in the buffer without cycling the buffer
const char* getNextMessage(bufferStruct* buffer);
// cycles to next message in the buffer
int cycleBuffer(bufferStruct* buffer);

#endif
