#ifndef GARBAGE_COLLECTOR_H_
#define GARBAGE_COLLECTOR_H_


#include "Connection.h"

class GarbageCollector{
    
    
    private:
    
    Connection* head; //pts to the first conn in the list
    Connection* tail;
    
    public:
    
    GarbageCollector();
    void addConnection(Connection* connection);
    void cleanup();
    void terminate();
    
    ~GarbageCollector();
};

#endif