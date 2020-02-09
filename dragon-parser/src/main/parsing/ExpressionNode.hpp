#ifndef EXPRESSIONNODE
#define EXPRESSIONNODE

#include <optional>
#include <vector>

class OperatorNode;
class TermNode;
#include "../commandline/TokenList.hpp"

using namespace std;

class ExpressionNode {

public:
	//DragonExpressionNode should be similar to jack expression
	//an expression should be anything that returns a value or computes to a value

	ExpressionNode(TokenList tkl, bool debug);
	ExpressionNode(TermNode* myterm);
	ExpressionNode(TermNode* leftTerm, OperatorNode* op, TermNode* rightTerm);


	void performTreeTransformation(
		vector<OperatorNode*> ops,
		vector<TermNode*> terms
	);

	TermNode* term1 = NULL;
	optional<OperatorNode*> op = nullopt;
	optional<TermNode*> term2 = nullopt;
};

#endif