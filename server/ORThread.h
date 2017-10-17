#ifndef ORTHREAD_H
#define ORTHREAD_H


#include "RuleBase.h"
#include "KnowledgeBase.h"
#include "Thread.h"

#include <iostream>
using namespace std;
class RuleBase;

struct orArgs{
    int                 leftOrRight; //0 or 1 depending on side
    int                 id;
    Query               paramQuery;
    Query               originalQuery;
    deque<Query>*       tOut;
    KnowledgeBase*      kbPtr;
    RuleBase*           rbPtr;
    pthread_mutex_t*    printmutex;
    
    orArgs(void * container){
        orArgs *unpack  = (orArgs*) container;
        id              = unpack->id;
        leftOrRight     = unpack->leftOrRight;
        paramQuery      = unpack->paramQuery;
        originalQuery   = unpack->originalQuery;
        tOut            = unpack->tOut;
        kbPtr           = unpack->kbPtr;
        rbPtr           = unpack->rbPtr;
        printmutex      = unpack->printmutex;
    }
    orArgs(){}
};

class ORThread : public Thread{
    private:
        struct orArgs args;
    public:
        
        ORThread(void *(*_threadRoutine) (void *) =NULL, void * arg = NULL);
        
        ~ORThread();
        void * threadMainBody(void *);
};

#endif