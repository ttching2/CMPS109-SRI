#include <iostream>
#include <string>
#include "Interface.h"
#include "socketInclude.h"
#include "GarbageCollector.h"
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[]) {
    
    //Interface interface;
    TCPServerSocket *tcpServerSocket = new TCPServerSocket("0.0.0.0", 2222, 100);
    if (tcpServerSocket->initializeSocket() == -1){
        cout << "\nERROR Initialize Socket";
    }
    cout << "Initialized";
    GarbageCollector * garbageCollector = new GarbageCollector();
    
    while (1){

        TCPSocket* tcpSocket = tcpServerSocket->getConnection();
        cout << "\nGot a new connection\n";
        fflush(stdout);
        if (tcpSocket == NULL){
            cout << "\nError occurred\n";
        }
        garbageCollector->cleanup();
        Connection* c = new Connection(tcpSocket);
        
        c->start();
        garbageCollector->addConnection(c);
    }

    delete garbageCollector;
    delete tcpServerSocket;
    return 0;
}