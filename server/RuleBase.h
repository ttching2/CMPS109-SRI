/* RuleBase.h */
//Rulebase which holds the rulebase datastructure for all the rules
#ifndef RULEBASE_H_
#define RULEBASE_H_

#include <map>
#include <deque>
#include <string>

#include "Query.h"
#include "KnowledgeBase.h"
#include "ORThread.h"
#include "ANDThreadR.h"
#include "ANDThreadL.h"

using std::map;
using std::string;
using std::vector;

//Struct for keeping track of matching variables in a rule.
typedef struct varPair{
    int origParam;
    int newParam;
    varPair(): newParam(-1){}
}varPairT;

class RuleBase{
	friend class Dumper;

private:
	map<string, deque<Query> > ruleContainer;						//A map with the Rule name as the key and a deque of 
																	//query objects to hold all the paramters that the rule has
	int							id;									//Thread ID counter
	pthread_mutex_t 			datamutex;							//ANDThreadR mutex for when they want to insert into the result deque
	pthread_mutex_t				printmutex;							//Mutex so that all the threads will not overlap print statements
public:
	int 	AddRule(Query query);									//Adds a rule into the database
	int 	RemoveRule(Query query);								//Removes a rule by name
	int 	QueryRule(Query query,deque<Query>& output,KnowledgeBase kb);//Deduce aliases to facts and logical operations
	
	bool	doesRuleExist(Query query);								//Check if the rule exists
	bool	setORRule(Query& query, string name);					//Check if the rule exists and passes the existing rule to the query
	bool    setRuleIdent(Query& query, string name);				//Sets the OR/AND identity for a query objec
	bool    setSecondIdent(Query& query, string name);				//Does a half copy of a rule into a query object
	Query	createFactQuery(string name, deque<string> parameters);	//Creates a fact inference to search the knowledge base with
	map<string,varPairT> setParamIndex(Query query);				//Finds the index of matching variables in a rule
	
	///void * orFunctionParallelized(void* orArgs); //parallel code
	
	
	RuleBase();
	~RuleBase();
};
#endif
