#include "socket/include/TCPSocket.h"
#include <iostream>


int main(){
    
    string input;

    TCPSocket socket("0.0.0.0", 2222, 65535, 65535);
    while (input != "EXIT"){
        
        cout << "$$> ";
        getline(cin, input);
        fflush(stdout);
        if (input.size() == 0){
            input.clear();
            fflush(stdin);
            continue;
        }
        if (socket.isPeerDisconnected() == true){
            cout << "\nDisconnected from server\n";
            break;
        }else {
            
            //Just open file on client side and pass each line as a command to the server
            if (input.find("LOAD") != string::npos){ //found LOAD command
                fflush(stdout);
                cout << "LOAD"; 
                string line;
                ifstream file(input.substr(5,string::npos));
                if(file.is_open()){
                    while(getline(file,line)){
                        if(socket.writeToSocket(line.c_str(), line.size()) != -1){
                            char readIn[65535];
                            memset(readIn, 0, 65535-1);
                
                            if (socket.readFromSocket(readIn, 65535-1) > 0){
                    
                                string output(readIn);
                                fflush(stdout);
                                cout << output;
                                fflush(stdout);
                    
                            }else {
                    
                                fflush(stdout);
                            }
                        }
                    }
                    file.close();
                }
                else{
                    cout<<"File name not found\n";
                }
                fflush(stdout);
            }else if (input.find("DUMP") != string::npos){ //found DUMP command
                fflush(stdout);
                //Better way to get filename?
                string file_name = input.substr(5); //Assumes a space and then the filename
                cout << "DUMP";
                //Do actual dumping here
                string buffer; //The string that will contain all of the content to write
                if (socket.writeToSocket(input.c_str(), input.size()) != -1){
                    char readIn[65535];
                            memset(readIn, 0, 65535-1);
                
                            if (socket.readFromSocket(readIn, 65535-1) > 0){
                    //Question: can this code get a lot in one string, or does it only get a small number of bytes?
                                string output(readIn);
                                fflush(stdout);
                                buffer = output;
                                fflush(stdout);
                    
                            }else {
                    
                                fflush(stdout);
                            }
                }
                ofstream output_file; //open file
                output_file.open(file_name, ios::trunc); //overwrite it if it exists
                output_file << buffer; //write the buffer to the sri file
                output_file.close(); //close file
                fflush(stdout);  
            }else if (socket.writeToSocket(input.c_str(), input.size()) != -1){

                char readIn[65535];
                memset(readIn, 0, 65535-1);
                
                if (socket.readFromSocket(readIn, 65535-1) > 0){
                    
                    string output(readIn);
                    fflush(stdout);
                    cout << output;
                    fflush(stdout);
                    
                }else {
                    
                    fflush(stdout);
                }
            }
        }
    }
    cout << "\nEXITING\n";
    socket.shutDown();
    
    return 0;
}