 
#include <string>
#include <antlr4-runtime/antlr4-runtime.h>
#include "egcBaseVisitor.h"


class  Visitor : egcBaseVisitor {
public:    
        virtual antlrcpp::Any visitFormula(egcParser::FormulaContext *ctx) override;
        virtual antlrcpp::Any visitPlusMinus(egcParser::PlusMinusContext *ctx) override;
        virtual antlrcpp::Any visitNumber(egcParser::NumberContext *ctx) override;

};
