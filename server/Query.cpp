#include "Query.h"

ostream& operator<<(ostream &output, const Query &query){
    output<<query.name<<"(";
    for(int i = 0; i < query.parameters.size(); i++){
        output<<query.parameters[i];
        if(i != query.parameters.size()-1) output<<",";
    }
    output<<")";
    return output;
}

bool Query::factEquality(const Query& other){
	
	bool isEqual = true;
	
	isEqual = (this->name == other.name);
	if (isEqual == false ) return isEqual;
	
	isEqual = (this->parameters == other.parameters);
	
	return isEqual;
}
