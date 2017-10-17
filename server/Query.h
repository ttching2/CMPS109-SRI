/* Query.h */
//Struct query object that holds all of the fact and rule data

#ifndef QUERY_H_
#define QUERY_H_

#include <deque>
#include <queue>
#include <iostream>

using namespace std;

struct Query{
	
	int 					flag;			//Signals the presence of variables in parameter list
	string					command;		//Name of the command issued, FACT, RULE ,INFERENCE
	string					ident;			//Rule or fact identifier 
	//For adding/removing/parsing
	string					name;			//Name of the rule or fact to add like Parent, Father
	deque<string>			parameters;		//The parameters of a fact or rule
	string					ruleParamName[2];//The names of the facts/rules that make up the rule and there will only be 2 of them ex. Mother Parent
	deque<deque<string> >	ruleParams;		//A queue of a queue to hold the rule parameters
	string					ruleIdent;		//String to hold whether the rule is an AND or OR
	//For dumping/loading
	string					file;			//Name of the file to output/load
	
	Query():flag(0){}						//Default constructor to set the flag to 0
	friend ostream& operator<<( ostream &output, const Query &query );//Operator overload to output query objects which is just in the form Father(Bill,John)

	bool factEquality(const Query& other);
};


#endif

