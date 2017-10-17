/* QueryOperations.h */
//Passes the query objects into the appropriate rule/knowledge base for commands
#ifndef QUERY_OPERATIONS_H_
#define QUERY_OPERATIOSN_H_
#include <deque>
#include "Dumper.h"
#include "KnowledgeBase.h"
#include "RuleBase.h"
#include "Query.h"

class QueryOperations{

private:
	Dumper dumper;
	KnowledgeBase kb;
	RuleBase rb;


public:
	QueryOperations();
	int Add(Query query); //Add fact or rule
	int Remove(Query query); //Remove fact and rule

	int Inference(Query query, deque<Query>& output); //inference the query
	int Dump(string &outputBuffer); //write to file
	~QueryOperations();
};
#endif
