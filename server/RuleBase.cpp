#include "RuleBase.h"


RuleBase::RuleBase(){
    id = 2;                                 //ID starts at 2 because the main program is on Thread 1
    pthread_mutex_init(&datamutex,NULL);
    pthread_mutex_init(&printmutex,NULL);
}

RuleBase::~RuleBase(){
    
}
	
/*
Adds a rule if it doesn't exist with the name in the rule base yet.
*/
int RuleBase::AddRule(Query query){
    if(!doesRuleExist(query))
        ruleContainer[query.name].push_back(query);
    
}

/*
Removes a rule from the rulebase if found.
*/
int RuleBase::RemoveRule(Query query){
    map<string, deque<Query> >::iterator it;
    it = ruleContainer.find(query.name);
    if (it != ruleContainer.end() ){
        ruleContainer[query.name].clear();
        ruleContainer.erase(it);
    }
}

/*
Returns whether a rule is found or not.
*/
bool RuleBase::doesRuleExist(Query query){
    
    map<string, deque<Query> >::iterator it;
    it = ruleContainer.find(query.name);
    if (it == ruleContainer.end() ){
        return false;
    }else {
        return true;
    }
}

/*
Searches the rule base for the rule name. If found it
sets the query object to be the rule found.
*/
bool RuleBase::setRuleIdent(Query& query, string name){
    map<string, deque<Query> >::iterator it;
    deque<string> temp = query.parameters;
    it = ruleContainer.find(name);
    if (it == ruleContainer.end() ){
        return false;
    }
    else if(query.command.compare("INFERENCE") == 0){
        query = ruleContainer[name][0];
        query.parameters = temp;
        return true;
    }
    else {
        query = ruleContainer[name][0];
        query.parameters[0] = temp[0];
        return true;
    }
}

/*
Searches for a rule if it exists after the 2nd half of the AND operator of a rule query.
If a rule is found set the query object to have the rule identifier set and the rule parameters.
Does not copy the complete rule found into the query object.
*/
bool RuleBase::setSecondIdent(Query& query, string name){
    map<string, deque<Query> >::iterator it;
    deque<string> temp = query.parameters;
    it = ruleContainer.find(name);
    if (it == ruleContainer.end() ){
        return false;
    }else {
        query.ruleIdent = ruleContainer[name][0].ruleIdent;
        query.ruleParamName[0] = ruleContainer[name][0].ruleParamName[0];
        query.ruleParamName[1] = ruleContainer[name][0].ruleParamName[1];
        query.ruleParams.push_back(temp);
        query.ruleParams.push_back(temp);
        return true;
    }
}

/*
Creates a rule query to be searched if the rule is found in the rule database.
The rule is made specifically for the OR operator in which the name of the rule to
search is copied in and the parameters are copied over from the original rule parameters
and the query object is returned.
*/
bool RuleBase::setORRule(Query& query, string name){
    map<string, deque<Query> >::iterator it;
    deque<string> temp = query.parameters;
    it = ruleContainer.find(name);
    if (it == ruleContainer.end() ){
        return false;
    }else {
        query = ruleContainer[name][0];
        query.parameters = temp;        //Technically only works for ORs
        query.ruleParams[0] = temp;
        query.ruleParams[1] = temp;
        return true;
    }
}

/*
Creates a query object that can be used by the knowledge based to query for a fact
setting the name parameter to be searched for and the list of parameters that need
to be matched.
*/
Query RuleBase::createFactQuery(string name, deque<string> parameters){
    Query query;
    query.name = name;
    query.parameters = parameters;
    for(int i = 0; i < parameters.size(); i++){
        if (parameters[i][0] == '$') {
            query.flag = 1; 
            break;
        }
    }
    return query;
}

/*
Recursive function to query rules
*/
int RuleBase::QueryRule(Query query, deque<Query>& output,KnowledgeBase kb){
    
    Query paramQuery;           //Temporary query used by OR to search for more rules
    deque<Query> threadOut;
    deque<Query> tOut; 
    deque<Query> tempOutput;    //Temporary output of the second half of the AND operator
    deque<Query> result;        //Temporary output of AND operator
    int count = 0;
    deque<string> tempOR;
    /*OR operator for a rule
    Create 2 threads which wil run in parallel. Then wait for both to finish before adding to the output deque.
    Either half of the rule needs to return true for the result to be outputted. Check first if the 
    name of the parameter is a rule and if it is recursively call the function again to break that one down.
    If it's a fact then search the knowledge base to output all of the facts that match the query
    */
    if(query.ruleIdent.compare("OR") == 0){
        //Create the struct to hold all the parameters to run the thread
        struct orArgs           rThread;
        rThread.leftOrRight     = 0;
        rThread.paramQuery      = paramQuery;
        rThread.originalQuery   = query;
        rThread.tOut            = &tOut;
        rThread.kbPtr           = &kb;
        rThread.rbPtr           = this;
        rThread.id              = id++;
        rThread.printmutex      = &printmutex;
        
        struct orArgs lThread;
        lThread.leftOrRight     = 1;
        lThread.paramQuery      = paramQuery;
        lThread.originalQuery   = query;
        lThread.tOut            = &tOut;
        lThread.kbPtr           = &kb;
        lThread.rbPtr           = this;
        lThread.id              = id++;
        lThread.printmutex      = &printmutex;
        
        ORThread a(NULL, &rThread);
        ORThread * thread = &a;
        ORThread b(NULL, &lThread);
        ORThread * thread2 = &b;
        
        thread->start();
        thread2->start();

        thread->waitForRunToFinish();
        thread2->waitForRunToFinish();
        for(int i = 0; i < tOut.size(); i++){
            output.push_back(tOut[i]);
        }
    }
    /*AND operator for a rule
    Create 1 thread to search the first half of the AND rule. As the result deque gets filled start running threads for each result to
    query the second half of the rule.
    The left half of the AND rule is the start of breaking down the rule. Just call it similar to the OR operator and return its output
    Loop through the output of the first half of the rule and replace the matching variable between both parameters with the output and check
    if it's a rule or a fact.If it's a rule recursively call it with the parameters set to the narrowed result. If it's a fact search the knowledgebase
    with the narrowed results. Compare both output and tempOutput and concatenate the results into the name of the rule and the differing variables.
    */
    else if(query.ruleIdent.compare("AND")==0){
        vector<Thread*> threadmanager;
        //map<string,varPairT> varmap = setParamIndex(query);
        map<int,vector<int> > tracker;
        int n = 0;
        int i = 0;
        //Create the struct to hold all the parameters to run the thread
        struct orArgs lThread;
        lThread.leftOrRight     = 0;
        lThread.paramQuery      = paramQuery;
        lThread.originalQuery   = query;
        lThread.tOut            = &threadOut;
        lThread.kbPtr           = &kb;
        lThread.rbPtr           = this;
        lThread.id              = id++;
        lThread.printmutex      = &printmutex;
        
        ANDThreadL b(NULL,&lThread);
        ANDThreadL * thread1 = &b;
        
        thread1->start();
        threadmanager.push_back(thread1);
        
        while(thread1->isRunning() || ( threadOut.size() != 0)){
            if(threadOut.size() > 0){
                tOut.push_back(threadOut.front());
                threadOut.pop_front();
                //Create the struct to hold all the parameters to run the thread
                struct andArgsr rThread;

                rThread.originalQuery       = query;
                rThread.inputQ              = tOut.back();
                rThread.kbPtr               = &kb;
                rThread.rbPtr               = this;
                rThread.tempOutput          = &tempOutput;
                rThread.result              = &result;
                rThread.datamutex           = &datamutex;
                rThread.id                  = id++;
                rThread.printmutex          = &printmutex;

                ANDThreadR * thread2 = new ANDThreadR(NULL, &rThread);
                thread2->start();
                threadmanager.push_back(thread2);

            }
        }
        //Wait for all the threads to finish so the result deque isn't incomplete
        for(int i = 0; i < threadmanager.size(); i++){
            threadmanager[i]->waitForRunToFinish();
        }
        //Push all the results into the output deque just in case theres already results saved in output
        for(int i = 0; i < result.size(); i++){
            output.push_back(result[i]);
        }
    }
    
}