/* KnowledgeBase.h */
//Knowledge base that holds all the Facts in a map data structure
#ifndef KNOWLEDGE_BASE_H_
#define KNOWLEDGE_BASE_H_

#include <string>
#include <map>
#include <deque>


//#include "Node.h"
#include "Query.h"

class KnowledgeBase{
	friend class Dumper;

private:

	map<string, deque<Query> > knowledgeContainer;			//Knowledge base datastructure that maps the Fact names as keys and inserts the query objects into a deque for each key
	
public:

	int AddFact(Query query);								//Adds a fact into the knowledge base data structure
	int RemoveFact(Query query);							//Removes a fact either by name or a specific fact
	int QueryFact(Query query, deque<Query>& inputDeque);	//Searches for a fact by name and parameters
	bool doesFactExist(Query query);						//Checks if a fact exists in the database

	KnowledgeBase();
	~KnowledgeBase();
};


#endif

