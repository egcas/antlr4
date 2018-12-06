#include "visitor.h"

using namespace std;
using namespace antlrcpp;
using namespace antlr4;

antlrcpp::Any Visitor::visitFormula(egcParser::FormulaContext *ctx)
{
    //tree::TerminalNode* node;
    size_t start = ctx->getStart()->getCharPositionInLine();
    Token* stopToken = ctx->getStop();
    size_t stop = stopToken->getCharPositionInLine();
    stop += stopToken->getText().length() - 1;
    cout << "formula: " << start << "/" << stop << endl;
    //node = ctx->getToken();
    //size_t charPos = node->getSymbol()->getCharPositionInLine();
            
    //antlrcpp::Any result = ctx->name()->NAME()->getText();
    
 //return result;
    return visitChildren(ctx);
} 


antlrcpp::Any Visitor::visitPlusMinus(egcParser::PlusMinusContext *ctx)
{
    size_t start = ctx->getStart()->getCharPositionInLine();
    Token* stopToken = ctx->getStop();
    size_t stop = stopToken->getCharPositionInLine();
    stop += stopToken->getText().length() - 1;
    cout << "plusminus: " << start << "/" << stop << endl;

  return visitChildren(ctx);
}

antlrcpp::Any Visitor::visitNumber(egcParser::NumberContext *ctx)
{
    size_t start = ctx->getStart()->getCharPositionInLine();
    Token* stopToken = ctx->getStop();
    size_t stop = stopToken->getCharPositionInLine();
    stop += stopToken->getText().length() - 1;

    cout << "number: " << start << "/" << stop << endl;

  return visitChildren(ctx);
}
