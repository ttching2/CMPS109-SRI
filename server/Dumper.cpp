#include "Dumper.h"
#include <iostream>
#include <fstream>
using namespace std;
    
Dumper::Dumper(){
    //intentionally empty
}

Dumper::~Dumper(){
    //intentionally empty
}

void Dumper::dump(string &buffer, KnowledgeBase& KB, RuleBase& RB){
    buffer = "";
    //First add all facts
    //Loop through each fact with the same name
    for(auto &outer_pair : KB.knowledgeContainer){ //string, vector<Query> pairs
        for(auto &query : outer_pair.second){
            buffer += "FACT ";
            buffer += query.name;//add query fields
            buffer += "(";
            //Loop through each parameter and add it to the buffer
            for(int i = 0; i < query.parameters.size(); i++){
                buffer += query.parameters[i];
                if(i+1 != query.parameters.size()){
                    buffer += ",";//add commas in between each one, but not for the last param
                }
            }
            buffer += ")\n"; //add newline
        }
    }
    //Then add all rules
    //Loop through each set of rules with the same name
    for(auto &outer_pair : RB.ruleContainer){ //string, vector<Query> pairs
        //Loop through each query in the vector of queries for the rule
        for(auto &query : outer_pair.second){
            buffer += "RULE ";
            buffer += query.name;//add query fields to the buffer
            buffer += "(";
            //Loop through each parameter and add it to the buffer
            for(int i = 0; i < query.parameters.size(); i++){
                buffer += query.parameters[i];
                if(i+1 != query.parameters.size()){
                    buffer += ","; //add commas in between each one, but not for the last param
                }
            }
            buffer += "):- ";
            buffer += query.ruleIdent; //This is "OR" or "AND"
            buffer += " ";
            //Add first sub-rule to the buffer
            buffer += query.ruleParamName[0];
            buffer += "(";
            //Just like before, print all params for the sub-rule
            for(int i = 0; i < query.ruleParams[0].size(); i++){
                buffer += query.ruleParams[0][i];
                if(i+1 != query.ruleParams[0].size()){
                    buffer += ",";
                }
            }
            buffer += ") ";
            //Repeat for second sub-rule
            buffer += query.ruleParamName[1];
            buffer += "(";
            for(int i = 0; i < query.ruleParams[1].size(); i++){
                buffer += query.ruleParams[1][i];
                if(i+1 != query.ruleParams[1].size()){
                    buffer += ",";
                }
            }
            buffer += ")\n"; //end the line
        }
    }
}