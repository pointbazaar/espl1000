#ifndef LOOPSTATEMENTNODE
#define LOOPSTATEMENTNODE

//standard headers
#include <vector>

//project headers
#include "../../ExpressionNode.hpp"
#include "../StatementNode.hpp"

using namespace std;

class LoopStatementNode{
public:
	ExpressionNode count;
	vector<StatementNode> statements;
};

#endif