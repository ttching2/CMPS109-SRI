/* Dumper.h */
//Dumper class to dump the knowledge base and rule base into a file
#ifndef DUMPER_H_
#define DUMPER_H_

#include <string>
#include "KnowledgeBase.h"
#include "RuleBase.h"

#include "Query.h"


class Dumper{

private:
    

public:
    Dumper();
    ~Dumper();
    void dump(string &outputBuffer, KnowledgeBase& KB, RuleBase& RB);    //Dumps the knowledge and rule base into a file
    
};

#endif