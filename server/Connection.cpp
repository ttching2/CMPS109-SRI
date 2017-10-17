#include "Connection.h"

Connection::Connection(TCPSocket* p_tcpSocket) : Thread()
{
    
    tcpSocket = p_tcpSocket;
    next_connection = NULL;
}

void Connection::setNextConnection(Connection* connection)
{
    next_connection = connection;
}

Connection* Connection::getNextConnection(){
    
    return next_connection;
}

Connection::~Connection(){
    
    if( tcpSocket != NULL){
        delete tcpSocket;
        
    }
}



void * Connection::threadMainBody(void * arg){
    
    char cmd[8192];
    memset(cmd, 0, 8192-1);
    
    while (1){
        int read_bytes = tcpSocket->readFromSocket(cmd, 8192-1);
            
        if (read_bytes > 0){
            //success.
            cout << "\nRead success\n";
            
            string command(cmd);
            string outputBuffer;
            
            if (command != "EXIT"){
                
                if (command.find("LOAD") != string::npos){ //found LOAD command
                    fflush(stdout);
                    cout << "LOAD";
                    //line 35, we have already read from the line. all you gotta do is take stuff in the line and put it into the kb and rb.
                    //modify load function.
                    
                    fflush(stdout);
                }else if (command.find("DUMP") != string::npos){ //found DUMP command
                    fflush(stdout);
                    cout << "DUMP";
                    //recieved dump command on line 35. all you gotta do is take everything from kb and rb and dump it into outputBuffer
                    //will be sent over the line on line 70
                    interface.executeCommand("DUMP", outputBuffer);
                    fflush(stdout);  
                }else{
                    
                    interface.executeCommand(command, outputBuffer);
                    cout << "Command processed: " << command << "\n";
                    cout << "Output: " << outputBuffer << "\n";
                    
                    outputBuffer.append("\n");
                    fflush(stdout);                    
                    
                }
                //send over the line.
                tcpSocket->writeToSocket(outputBuffer.c_str(), outputBuffer.size());
            }else {
                
                tcpSocket->shutDown();
                return NULL;
            }
        }
    }
}
