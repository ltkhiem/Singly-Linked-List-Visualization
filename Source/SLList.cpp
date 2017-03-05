#include "SLList.h"

void SLList::Init()
{
    pHead = NULL;
    pTail = pHead;
}

void SLList::Append(std::string s)
{
    n++;
    if (n==1)
    {
        pTail = new Node;
        pTail -> data = s;
        pTail -> pNext = NULL;
        pTail -> index = n;
        pHead = pTail;
    }
    else
    {
        pTail -> pNext = new Node;
        pTail = pTail -> pNext;
        pTail -> data = s;
        pTail -> index = n;
        pTail -> pNext = NULL;
    }
}

int SLList::GetSize()
{
    return n;
}

void SLList::InitScan()
{

    pScan = pHead;
}

void SLList::ScanWholeList()
{
    pScan = pScan -> pNext;
    if (pCur=NULL) return;
}

std::string SLList::GetCurrentData()
{
    return (pScan->data);
}

int SLList::Search(std::string s)
{
    Node *pSch=pHead;
    while (pSch!=NULL)
    {
        if (pSch->data==s) return pSch->index;
        pSch=pSch->pNext;
    }
    return -1;
}

void SLList::RemoveAll()
{
    Node *pRemove;
    while (pHead!=NULL)
    {
        pRemove = pHead;
        pHead=pHead->pNext;
        delete pRemove;
    }
    n=0;
}

void SLList::Insert(std::string s, int pos)
{
    Node *pIns, *pTmp, *pPrev;
    if (pos==0)
    {
        pIns = new Node;
        pIns->data = s;
        pIns->pNext = pHead;
        pHead = pIns;
        n++;
        return;
    }
    pIns = pHead;
    while (pIns!=NULL)
    {
        if (pos==0)
        {
            pTmp = pIns;
            pIns = new Node;
            pIns->data = s;
            pIns->pNext = pTmp;
            if (pPrev!=NULL) pPrev->pNext=pIns;
            n++;
            return;
        }
        pPrev=pIns;
        pIns = pIns->pNext;
        pos--;
    }
    pIns = new Node;
    pIns->data = s;
    pIns->pNext = NULL;
    pTail->pNext = pIns;
    n++;
}

bool SLList::LoadFile(char* path){
    n=0;
    std::ifstream fin;
    bool success=false;
    bool ok=true;
    fin.open(path);
    if (!fin.is_open())
        return success;

    std::string s;
    getline(fin,s);
    while (s.size()>9){
        getline(fin,s);
        s.size()<=9 ? ok=true:ok=false;
    }
    if (ok){
        pHead=new Node;
        pHead->data=s;
        pHead->index=++n;
        pHead->pNext=NULL;
        pTail=pHead;
        Node* pCur=pHead;
        while (getline(fin,s)){
            if (s.size()<=9){
                pCur->pNext=new Node;
                pCur=pCur->pNext;
                pCur->data=s;
                pCur->index=++n;
                pCur->pNext=NULL;
                pTail=pCur;
            }
        }
        fin.close();
        success=true;
    }
    return success;
}

bool SLList::SaveFile(char* path){
    std::ofstream fout;
    bool success=false;
    fout.open(path);
    if (!fout.is_open())
        return success;
    if (pHead){
        Node* pCur=pHead;
        while (pCur){
            fout<<pCur->data<<std::endl;
            pCur=pCur->pNext;
        }
        success=true;
    }
    fout.close();
    return success;
}

bool SLList::isEmpty(){
    return (pHead ? false:true);
}

int SLList::Find(std::string s){
    if (pHead){
        int cc=0;
        if (pHead->data==s)
            return cc;
        Node* pCur=pHead;
        while (pCur->pNext){
            if (pCur->pNext->data==s)
                return cc+1;
            cc++;
            pCur=pCur->pNext;
        }
    }
    return -1;
}

void SLList::Delete(int pos)
{
    Node *pDel, *pNow, *pPre;
    if (pos==0)
    {
        pDel=pHead;
        pHead=pHead->pNext;
        delete pDel;
        n--;
        return;
    }
    pPre=pHead;
    pNow=pHead->pNext;
    while (pNow!=NULL)
    {
        pos--;
        if (pos==0)
        {
            pDel=pNow;
            pNow=pNow->pNext;
            pPre->pNext=pNow;
            delete pDel;
            return;
        }
        pPre=pNow;
        pNow=pNow->pNext;
    }
}

std::string SLList::GetValue(int pos){
    if (pHead){
        Node* pTmp=pHead;
        int cc=0;
        while (pTmp && cc!=pos){
            pTmp=pTmp->pNext;
            cc++;
        }
        return pTmp->data;
    }
}

void SLList::DeleteX(int pos){
    if (pHead){
        if (pos==0){
            n--;
            Node* pTmp=pHead;
            pHead=pHead->pNext;
            delete pTmp;
            if (!pHead){
                pTail=NULL;
                return;
            }
            return;
        }
        Node* pCur=pHead;
        int cc=1;
        while (pCur->pNext && cc!=pos){
            pCur=pCur->pNext;
            cc++;
        }
        Node* pDel=pCur->pNext;
        pCur->pNext=pCur->pNext->pNext;
        if (pCur->pNext==NULL)
            pTail=pCur;
        delete pDel;
        n--;
    }
}

std::string SLList::Pop(){
    if (pHead){
        Node* pTmp=pHead;
        std::string s=pTail->data;
        while (pTmp->pNext && pTmp->pNext!=pTail)
            pTmp=pTmp->pNext;
        pTmp->pNext=NULL;
        if (pTail!=pHead){
            delete pTail;
            pTail=pTmp;
        }
        else{
            delete pHead;
            pHead=NULL;
            pTail=NULL;
        }
        n--;
        return s;
    }
}

void SLList::Insert_Head(std::string s){
    Node* pNew=new Node;
    pNew->data=s;
    pNew->pNext=pHead;
    pNew->index=0;
    pHead=pNew;
    n++;
    Node* pTmp=pHead;
    int cc=0;
    while (pTmp){
        pTmp->index=cc;
        cc++;
        pTmp=pTmp->pNext;
    }
}

