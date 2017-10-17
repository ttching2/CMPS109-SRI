#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "Thread.h"
#include "socket/include/TCPSocket.h"
#include "Interface.h"

class Connection: public Thread
{
    
    private:
        
        TCPSocket* tcpSocket;
        Connection* next_connection;
        
        Interface interface;
        string Dump();
    public:
    

    
    Connection(TCPSocket * p_tcpSocket);
    void * threadMainBody(void* arg);
    void setNextConnection(Connection* connection);
    void clearOutputBuffer();
    
    Connection* getNextConnection();
    ~Connection();
};

#endif