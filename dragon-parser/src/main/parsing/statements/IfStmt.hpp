#ifndef IFSTMT
#define IFSTMT

#include <vector>

#include "../../commandline/TokenList.hpp"

struct Expr;
class Stmt;

struct IfStmt{
	struct Expr* condition = NULL;
	std::vector<Stmt*>* statements = NULL;
	std::vector<Stmt*>* elseStatements = NULL;
};
	
struct IfStmt* makeIfStmt(TokenList* tokens,bool debug);

#endif