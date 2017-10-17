#include "Parser.h"


using namespace std;

Parser::Parser(){}

//Split function needed to split the input into seperate tokens
//Split function found here
//http://www.cplusplus.com/faq/sequences/strings/split/
template <typename Container>
Container& split(
  Container&                            result,
  const typename Container::value_type& s,
  const typename Container::value_type& delimiters)
{
  result.clear();
  size_t current;
  size_t next = -1;
  do
  {
    next = s.find_first_not_of( delimiters, next + 1 );
    if (next == Container::value_type::npos) break;
    next -= 1;
    
    current = next + 1;
    next = s.find_first_of( delimiters, current );
    result.push_back( s.substr( current, next - current ) );
  }
  while (next != Container::value_type::npos);
  return result;
}

Query Parser::parse(string input){
    Query query;
    
    vector <string> params;
    //Split input into the a vector each split by whitespace commas and parenthesis
    split(params, input, " ,()");
    
    if(params.size() == 0){
         throw 55; //Arbitrary error number for invalid command
    }
    //The first parameter of the string should be the command so set that to the query object
    //and give it to the right parse command passing the rest of the parameters.
    //Could probably try catch an error here for unknown command
    query.command = params[0];
    if(query.command.compare("FACT") == 0)
        parseFact(params, query);
    else if(query.command.compare("RULE") == 0)
        parseRule(params, query);
    else if(query.command.compare("DUMP") == 0)
        parseDump(params, query);
    else if(query.command.compare("DROP") == 0)
        parseDrop(params, query);
    else if(query.command.compare("LOAD") == 0)
        parseLoad(params, query);
    else if(query.command.compare("INFERENCE") == 0)
        parseInference(params, query);
    else if(query.command.compare("EXIT") == 0){
        return query;
    }
    else
        cout<<"Error improper input"<<endl;
    return query;
}

//Index 0 is the command name
//Index 1 is the file name
Query& Parser::parseDump(vector<string> input, Query& query){
    query.file = input[1];
    return query;
}

//Index 0 is the command name
//Index 1 is the name of the fact to be added
//Rest of the indices are parameters for the fact
Query& Parser::parseFact(vector<string> input, Query& query){
    query.ident = "FACT";
    query.name = input[1];
    for(int i = 2; i < input.size(); i++){
        if(input[i][0] == '#') break;
        query.parameters.push_back(input[i]);
    }
    return query;
}

//Index 0 is the command name
//Index 1 is the name of the rule or fact to be searched
//Rest of the indices are parameters
Query& Parser::parseInference(vector<string> input, Query& query){
    query.name = input[1];
    for(int i = 2; i < input.size(); i++){
        if(input[i][0] == '#') break;
        if(input[i][0] == '$') query.flag = 1;
        query.parameters.push_back(input[i]);
    }
    return query;
}

Query& Parser::parseLoad(vector<string> input, Query& query){
    if(input.size() > 1)
        query.file = input[1];
    else
        throw 55;
    return query;
}

//Index 0 is the command name
//Index 1 is the name of the rule
//The first portion is the name of the rule and its parameters
//The second portion is the AND or OR operator
//The last two portions are the rules/facts that make up the rule
Query& Parser::parseRule(vector<string> input, Query& query){
    query.ident = "RULE";
    query.name  = input[1];
    deque<string> temp;                                 //Temoporary string queue to hold the parameters for the rule definition
    int j       = 0;                                    //A counter to keep track of what part of the input we are on
    for(int i = 2; i < input.size(); i++){
        if(input[i][0] == '#') break;
        if(j == 0){
            if(input[i].compare(":-") == 0) j++;        //If the end of the parameters for the rule name is done go to the next portion
            else    query.parameters.push_back(input[i]);
        }
        else if(j == 1){
            query.ruleIdent = input[i];                 //The next part is the AND or OR operator
            j++;
        }
        else if(j == 2){
            query.ruleParamName[0] = input[i];          //Gets the name of the first rule parameter
            j++;
        }
        else if(j == 3){
            if(input[i][0] == '$')  temp.push_back(input[i]);//Adds the parameters of the first fact/rule to a temp queue and adds it when it reaches the end
            else{    
                query.ruleParamName[1] = input[i];      //Sets the name of the second parameter and empties out the temp queue
                query.ruleParams.push_back(temp); 
                deque<string> empty;
                empty.swap(temp);
                j++;
            }
        }
        else if(j == 4){
            if(input[i][0] == '$')  temp.push_back(input[i]);//Adds the parameters for the second fact/rule to the temp queue and will add it to the query object
        }
    }
    query.ruleParams.push_back(temp);
    return query;
}

//Index 0 is the command name
//The rest of the indices is the name of the facts or rules to drop
Query& Parser::parseDrop(vector<string> input, Query& query){
    query.name = input[1];
    for(int i = 2; i < input.size(); i++){
        if(input[i][0] == '#') break;
        if(input[i][0] == '$') query.flag = 1;
        query.parameters.push_back(input[i]);
    }
    return query;
}

Parser::~Parser(){}