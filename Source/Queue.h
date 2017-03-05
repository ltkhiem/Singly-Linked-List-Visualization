#pragma once
#include "header.h"

class LQueue
{
    private :
        int data[100];
        int top=0, pos=1, qsize=0;
    public :
        void Push(int val);
        int Pop();
        bool isEmpty();
        int GetFirst();
};
