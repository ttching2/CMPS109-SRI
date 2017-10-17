/* KnowledgeBase.cpp */
#include "KnowledgeBase.h"
#include <iostream>

KnowledgeBase::KnowledgeBase(){
    
}

int KnowledgeBase::AddFact(Query query){
    
    knowledgeContainer[query.name].push_back(query);
    
    return 1; //no error checking for now
}

int KnowledgeBase::RemoveFact(Query query){
    
    if (query.parameters.size() == 0){
        
        knowledgeContainer[query.name].clear();
        knowledgeContainer.erase(query.name);
    }else if (query.parameters.size() > 0){
        deque<Query> queriesToMatchToDelete;
        QueryFact(query, queriesToMatchToDelete);
        
        deque<Query>* kbFacts = &knowledgeContainer[query.name];
        int j = 0;
        for (auto i = kbFacts->begin(); i != kbFacts->end(); i++){
            
            for (int j = 0; j < queriesToMatchToDelete.size(); j++){
                
                Query tempQ = *i;
                if ( tempQ.factEquality(queriesToMatchToDelete[j]) == true ){
                    
                    kbFacts->erase(i);
                    break;
                }
            }
            if (kbFacts->size() == 0){
                knowledgeContainer.erase(query.name);
                break;
            }
        }
        return 1;
    }
        
    return 1;

    
}
bool KnowledgeBase::doesFactExist(Query query){

    map<string, deque<Query> >::iterator it;
    it = knowledgeContainer.find(query.name);
    if (it == knowledgeContainer.end() ){
        
        return false;
    }else {
        
        return true;
    }
}


int KnowledgeBase::QueryFact(Query query, deque<Query>& inputDeque){
    
    //check for empty input deque, and remove duplicate
    
    deque<Query>* tempDeque = &knowledgeContainer[query.name];
    deque<Query> retDeque;
    deque<Query> retDequeVarTemp;
    for(int i = 0; i < tempDeque->size(); i++){
        Query* tempQuery = &(*tempDeque)[i];
        if (tempQuery->parameters.size() == query.parameters.size() ){
            
            retDeque.push_back(*tempQuery); //build a deque of all queries of same size
        }
    }
    //after obtainin all things in same size, count number of variables and their positions. if there are variables
    if (query.flag == 1){
        
        map<string, string > varTable;
        
        bool goodToReturn = true;
        Query tempQ;
        for (int i = 0; i < retDeque.size(); i++){
            
            for (int j = 0; j < query.parameters.size(); j++){
                tempQ = retDeque[i];
                if ( query.parameters[j][0] == '$' ){
                    
                    std::map<string,string>::iterator it;
                    it = varTable.find(query.parameters[j]);
                    if (it == varTable.end()){
                        
                        
                        varTable[query.parameters[j]] = tempQ.parameters[j];                        
                    }else { //parameter was found. match. if match we good. no match, false ret
                        
                        string parameterValue = varTable[query.parameters[j]];
                        if (parameterValue != tempQ.parameters[j] ){
                            
                            goodToReturn = false;
                        }
                    } //example, Bob($x, $x) mapped to Bob(John, John). 2 iterations wont hit goood to return = false
                        //but Bob($x, $x) mapped to Bob(John, Jack). first iteration good. second iteration, will hit else and good return = false will hit.
                        //when using all different vars, no way that find can find success. so never good to return will hit
                    //last step, look for duplicates in second.
                    
                }else{  //not a parameter to match
                    
                    if ( (tempQ.parameters[j]) != query.parameters[j] ){
                        
                        goodToReturn = false;
                    }
                }
            }
            //look for duplicates in second
            for (auto i = varTable.begin(); i != varTable.end(); i++){
                
                auto j = varTable.begin();
                j++;
                for (; j != varTable.end(); j++){
                    
                    if ( (i->second == j->second ) && (i->first != j->first) ){
                        goodToReturn = false;
                    }
                }
            }
            if (goodToReturn){
                
                retDequeVarTemp.push_back(tempQ);
            }
            goodToReturn = true;
            varTable.clear();
        }
    }else{
        
        bool goodToReturn = true;
        Query tempQ;
        for (int i = 0; i < retDeque.size(); i++){
            
            for (int j = 0; j < query.parameters.size(); j++){
                tempQ = retDeque[i];
                if ( (tempQ.parameters[j]) != query.parameters[j] ){
                    goodToReturn = false;
                }
            }
            if (goodToReturn){
                
                retDequeVarTemp.push_back(tempQ);
            }
            goodToReturn = true;
        }
        
    }
    
    bool canIAddToInput = true;
    for (auto i = retDequeVarTemp.begin(); i != retDequeVarTemp.end(); i++){
        
        for (auto j = inputDeque.begin(); j != inputDeque.end(); j++){
            
            if ( (*i).factEquality(*j) == true ){
                canIAddToInput = false;
            }
        }
        if (canIAddToInput == true){
            inputDeque.push_back(*i);
        }
        canIAddToInput = true;
    }
    
    return 1;
}

KnowledgeBase::~KnowledgeBase(){}

/*Code trashcan, dont delete untill confirm working
Query trash code: http://pastebin.com/wDq6593z
remove fact trash: http://pastebin.com/FGYMFvGu
add fact trash: http://pastebin.com/pWYycsDe
remove fact deprecated 2: http://pastebin.com/0pgWG4QT
getxpos: http://pastebin.com/9KRZ4XH3
*/
