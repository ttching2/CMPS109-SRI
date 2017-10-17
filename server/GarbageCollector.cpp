#include "GarbageCollector.h"

GarbageCollector::GarbageCollector(){
    head = tail = NULL;
}

void GarbageCollector::addConnection(Connection* connection){
    
    if (head == NULL)
        head = tail = connection;
    else {
        
        tail->setNextConnection(connection);
        tail = connection;
    }
}


void GarbageCollector::cleanup(){
    
    Connection* cur = head;
    
    for (; (cur != NULL) && !cur->isRunning(); ){
        
        cur->waitForRunToFinish();
        Connection* cur1 = cur;
        cur = cur->getNextConnection();
        delete (cur1);
    }
    
    head = cur;
    if (head == NULL)
        tail = NULL;
        
}

void GarbageCollector::terminate(){
    
    Connection *cur = head;
    for ( ; cur != NULL ; ){
        
        cur->waitForRunToFinish();
        Connection * cur1 = cur;
        cur = cur->getNextConnection();
        delete(cur1);
    }
    head = tail = NULL;
}

GarbageCollector::~GarbageCollector(){
    terminate();
}