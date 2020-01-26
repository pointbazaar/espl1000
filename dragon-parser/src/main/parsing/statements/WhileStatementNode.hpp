
//standard headers
#include <vector>

//project headers

using namespace std;

class WhileStatementNode  {
public:
	ExpressionNode condition;
	vector<StatementNode> statements;
};