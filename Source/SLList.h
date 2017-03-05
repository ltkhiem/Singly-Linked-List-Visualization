#pragma once
#include "header.h"

struct Node
{
    int index=0;
    Node *pNext=NULL;
    std::string data;
};

class SLList
{
    private :
        Node *pHead=NULL, *pCur=NULL, *pTail=NULL, *pScan;
        int n=0;
    public :
        void Init();
        void Append(std::string s);
        int Search(std::string s);
        void Delete(std::string s);
        void Delete(int pos);
        void Insert(std::string s, int pos);
        void RemoveAll();
        void InitScan();
        std::string GetCurrentData();
        void ScanWholeList();
        int GetSize();
        bool isEmpty();
        int Find(std::string s);
        bool SaveFile(char* path);
        bool LoadFile(char* path);
        void DeleteX(int pos);
        std::string GetValue(int pos);
        std::string Pop();
        void Insert_Head(std::string s);
};
