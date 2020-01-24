#ifndef RETURNSTATEMENTNODE
#define

using namespace std;

class ReturnStatementNode{

public:
	ExpressionNode returnValue;
	ReturnStatementNode(TokenList list);
};

#endif