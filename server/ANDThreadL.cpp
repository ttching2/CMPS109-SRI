#include "ANDThreadL.h"
#include <iostream>

using namespace std;

//Constructor which just passes the thread routine to the thread base class
ANDThreadL::ANDThreadL(void *(*_threadRoutine) (void *), void * args):Thread(_threadRoutine),andArgsl(args){

}

void * ANDThreadL::threadMainBody(void *){
    pthread_mutex_lock(andArgsl.printmutex);
    cout<<"Starting ANDThreadL Thread "<<andArgsl.id<<endl;
    pthread_mutex_unlock(andArgsl.printmutex);
    
    deque<string> tempOR;
    andArgsl.paramQuery.command = "INFERENCE";
    andArgsl.paramQuery.parameters.push_back(andArgsl.originalQuery.parameters[0]);
    andArgsl.paramQuery.parameters.push_back(andArgsl.originalQuery.ruleParams[0][1]);
   //First half of the AND operator is the same as the OR operator just call it searching for either a rule or fact
    if(andArgsl.rbPtr->setRuleIdent(andArgsl.paramQuery, andArgsl.originalQuery.ruleParamName[0]))
        andArgsl.rbPtr->QueryRule((andArgsl.paramQuery), *(andArgsl.tOut), *(andArgsl.kbPtr));
    else{
        //Queries the rule base with the same first parameter of the inference
        tempOR.push_back(andArgsl.originalQuery.parameters[0]);
        tempOR.push_back(andArgsl.originalQuery.ruleParams[0][1]);
        
        andArgsl.kbPtr->QueryFact( andArgsl.rbPtr->createFactQuery(andArgsl.originalQuery.ruleParamName[0], tempOR), *(andArgsl.tOut));
        
    }
    pthread_mutex_unlock(&mutex);
}

ANDThreadL::~ANDThreadL(){
    pthread_mutex_lock(andArgsl.printmutex);
    cout<<"Ending ANDThreadL Thread "<<andArgsl.id<<endl;
    pthread_mutex_unlock(andArgsl.printmutex);
}