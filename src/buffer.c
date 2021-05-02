#include "buffer.h"

char* bufferNextMessage(bufferStruct* buffer)
{
    int addId;
    addId = (buffer->bufferSize + buffer->readId)%BUFFER_SIZE;
    if(buffer->bufferSize < BUFFER_SIZE)
    {
        buffer->bufferSize++;
    }
    else
    {
        buffer->readId = (++buffer->readId)%BUFFER_SIZE;
    }
    return buffer->bufferArr[addId];
}

const char* getNextMessage(bufferStruct* buffer)
{
    return buffer->bufferArr[buffer->readId];
}

int cycleBuffer(bufferStruct* buffer)
{
    if(buffer->bufferSize > 0)
    {
        buffer->bufferSize--;
        buffer->readId = (++buffer->readId)%BUFFER_SIZE;
        return 0;
    }
    return -1;
}
