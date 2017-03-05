#include "Queue.h"

void LQueue::Push(int val)
{
    qsize++;
    top++;
    if (top>99)
    {
        if (pos>1)
            top = 1;
    }
    data[top] = val;
}

int LQueue::Pop()
{
    qsize--;
    pos++;
    if (pos>99) pos=1;
    return data[top-1];
}

bool LQueue::isEmpty()
{
    return(qsize==0);
}

int LQueue::GetFirst()
{
    return data[pos];
}
