//
// Created by kelvinsilva on 2/21/17.
//

#include "Interface.h"

Interface::Interface(){
    
}

int Interface::executeCommand(string command, string &outputBuffer){
    //Parse command
    Query query; 
    try{
        query = parser.parse(command);
    }
    catch (int ex){
        if(ex == 55){
            cout << "Error: invalid command" << endl;
        }
        return 0;
    }
    //Check the header of the query object and check what command it calls
    //If statements for all the functions to call with whatever else needs to be done
    if( (query.command.compare("FACT") == 0) || (query.command.compare("RULE") == 0) ) ops.Add(query);
    else if(query.command.compare("DROP") == 0) ops.Remove(query);
    else if(query.command.compare("DUMP") == 0) ops.Dump(outputBuffer);
    else if(query.command.compare("LOAD") == 0) Load(query, outputBuffer);
    else if(query.command.compare("INFERENCE") == 0){
        deque<Query> tempDeque;
        if (ops.Inference(query, tempDeque) == 0){
            return 0;
        }
        //print(tempDeque);
        stringstream tempBuff;
        printBuffer(tempDeque, tempBuff );
        outputBuffer = tempBuff.str();
    } 
    return 1;
}

void Interface::printBuffer(deque<Query> queries, stringstream& ss){
    
    for(int i = 0; i < queries.size(); i++){
        ss<<queries[i]<<'\n';
    }
    
}

//Loads a file and each line should just be a command so take each line and give it back to the execute command function
int Interface::Load(Query query, string& outputBuffer){
    string line;
    ifstream file(query.file);
    if(file.is_open()){
        while(getline(file,line)){
            if(line == " ") continue; //Maybe unneeded
            executeCommand(line, outputBuffer);
        }
        file.close();
    }
    else cout << "Unable to open file"; 
}

//Prints the results of the inference command
void Interface::print(deque<Query> queries){
    for(int i = 0; i < queries.size(); i++){
        cout<<queries[i]<<endl;
    }
}

Interface::~Interface(){}