#ifndef IFSTMT
#define IFSTMT

#include <vector>

#include "../../commandline/TokenList.hpp"

struct Expr;
class Stmt;

struct IfStmt{
	struct Expr* condition = NULL;
	std::vector<Stmt*> statements = vector<Stmt*>();
	std::vector<Stmt*> elseStatements = vector<Stmt*>();
};
	
struct IfStmt* makeIfStmt(TokenList* tokens,bool debug);

#endif