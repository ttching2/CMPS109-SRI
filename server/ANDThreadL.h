
#ifndef ANDTHREADL_H
#define ANDTHREADL_H

#include <iostream>
#include "Thread.h"
#include "RuleBase.h"
#include "KnowledgeBase.h"

using namespace std;
class RuleBase;

struct andArgs{
    int                 leftOrRight; //0 or 1 depending on side
    int                 id;
    Query               paramQuery;
    Query               originalQuery;
    deque<Query>*       tOut;
    KnowledgeBase*      kbPtr;
    RuleBase*           rbPtr;
    pthread_mutex_t*    printmutex;
    andArgs(void * container){
        andArgs *unpack  = (andArgs*) container;
        id              = unpack->id;
        leftOrRight     = unpack->leftOrRight;
        paramQuery      = unpack->paramQuery;
        originalQuery   = unpack->originalQuery;
        tOut            = unpack->tOut;
        kbPtr           = unpack->kbPtr;
        rbPtr           = unpack->rbPtr;
        printmutex      = unpack->printmutex;
    }
    andArgs(){}
};

class ANDThreadL: public Thread{
    
    private:
        struct andArgs andArgsl;
    public:
        ANDThreadL(void *(*_threadRoutine) (void *) =NULL, void * args = NULL);
        ~ANDThreadL();
        void * threadMainBody(void *);
};

#endif