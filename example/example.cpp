#include <iostream>

#include <antlr4-runtime/antlr4-runtime.h>
#include "egcLexer.h"
#include "egcParser.h"
#include "visitor.h"

using namespace std;
using namespace antlr4;

class EgcNode {
public:
        int test;
};

int main(int argc, const char* argv[]) {
    std::ifstream stream;
    stream.open("formula.txt");
    
    ANTLRInputStream input(stream);
    egcLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    egcParser parser(&tokens);    

    egcParser::FormulaContext* tree = parser.formula();

    Visitor visitor;
    antlrcpp::Any nodeTree = visitor.visitFormula(tree);

    return 0;
} 
