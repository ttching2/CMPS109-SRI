#include "ORThread.h"

#include <iostream>

using namespace std;

void * ORThread::threadMainBody (void *){
    pthread_mutex_lock(args.printmutex);
    cout<<"Starting ORThread Thread "<<args.id<<endl;
    pthread_mutex_unlock(args.printmutex);
    
    if(args.rbPtr->setORRule(args.paramQuery, args.originalQuery.ruleParamName[args.leftOrRight]))
                    
        args.rbPtr->QueryRule(args.paramQuery, *(args.tOut), *(args.kbPtr));
    else
        (*(args.kbPtr)).QueryFact(args.rbPtr->createFactQuery(args.originalQuery.ruleParamName[args.leftOrRight], args.originalQuery.parameters),*(args.tOut));
    pthread_mutex_unlock(&mutex);               
}

//Constructor which just passes the thread routine to the thread base class
ORThread::ORThread(void *(*_threadRoutine) (void *), void * arg):Thread(_threadRoutine), args(arg){}

ORThread::~ORThread(){
    pthread_mutex_lock(args.printmutex);
    cout<<"Ending ORThread Thread "<<args.id<<endl;
    pthread_mutex_unlock(args.printmutex);
}
