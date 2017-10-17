/* Parser.h */
//Breaks up the user input and inserts the data into a query object
#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>
#include <cstddef>
#include <vector>
#include <deque>
#include <queue>
#include <string>

#include "Query.h"

class Parser{

private:
    /*
    Splitting up the parsing this way will make it easier to code. 
    if all functionality is in public parse method, then this method will have too many lines to be manageable
    */
    Query& parseDump(vector<string> input, Query& query);      //Parses the input for the file name
    Query& parseFact(vector<string> input, Query& query);      //Parses for the fact's name and its parameters
    Query& parseInference(vector<string> input, Query& query); //Parses for the name to search and possibly a specific one
    Query& parseLoad(vector<string> input, Query& query);      //Parses for the name of the file to load
    Query& parseRule(vector<string> input, Query& query);      //RULE Parent($X,$Y):- OR Father($X,$Y) Mother($X,$Y)
    Query& parseDrop(vector<string> input, Query& query);      //Parses for the name of the rule/fact to drop
    

public:
    Parser();
    Query parse(string input);                                  //Initial parse call, breaks up the command into a vector and passes it to the proper parser function for further parse
    ~Parser();
	
};

#endif
