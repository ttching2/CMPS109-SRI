
#ifndef ANDTHREADR_H
#define ANDTHREADR_H

#include "Thread.h"
#include "RuleBase.h"
#include "KnowledgeBase.h"
#include <iostream>
#include <deque>

using namespace std;
class RuleBase;
typedef struct varPair varPairT;


struct andArgsr{
    int                     leftOrRight; //0 or 1 depending on side
    int                     id;
    Query                   originalQuery;
    Query                   inputQ;
    KnowledgeBase*          kbPtr;
    RuleBase*               rbPtr;
    deque<Query>*           tempOutput;
    deque<Query>*           result;
    pthread_mutex_t*        datamutex;
    pthread_mutex_t*        printmutex;
   
    
    andArgsr(void * container){
        andArgsr *unpack  = (andArgsr*) container;
        id              = unpack->id;
        originalQuery   = unpack->originalQuery;
        inputQ          = unpack->inputQ;
        kbPtr           = unpack->kbPtr;
        rbPtr           = unpack->rbPtr;
        tempOutput      = unpack->tempOutput;
        result          = unpack->result;
        datamutex       = unpack->datamutex;
        printmutex      = unpack->printmutex;
    }
    andArgsr(){}
};

class ANDThreadR: public Thread{
    private:
        struct andArgsr andArgsR;
         
    public:
        ANDThreadR(void *(*_threadRoutine) (void *) =NULL, void * args = NULL);
        ~ANDThreadR();
        void * threadMainBody(void * args);
};

#endif