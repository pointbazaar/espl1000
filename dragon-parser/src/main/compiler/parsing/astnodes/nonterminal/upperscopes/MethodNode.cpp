
//standard headers
#include <vector>
#include <set>

//project headers
#include "MethodNode.hpp"

class MethodNode : IASTNode {

public:
	bool isPublic = true;
	bool hasSideEffects;
	TypeNode returnType;
	string methodName;
	vector<DeclaredArgumentNode> arguments;
	vector<StatementNode> statements;

	MethodNode(TokenList tokens, bool debug) throws Exception {
		if (debug) {
			System.out.println("try to parse " + this.getClass().getSimpleName() + " from '" + tokens.toSourceCodeFragment() + "'");
		}

		TokenList copy = tokens.copy();

		copy.expectAndConsumeOtherWiseThrowException(new FnToken());

		this.methodName = new IdentifierNode(copy).identifier;

		copy.expectAndConsumeOtherWiseThrowException(new LParensToken());

		//while there is no ')' up, continue parsing arguments
		IToken next = copy.get(0);
		while (!(next instanceof RParensToken)) {
			if (this.arguments.size() > 0) {
				copy.expectAndConsumeOtherWiseThrowException(new CommaToken());
			}
			this.arguments.add(new DeclaredArgumentNode(copy));
			next = copy.get(0);
		}

		copy.expectAndConsumeOtherWiseThrowException(new RParensToken());

		if (copy.head() instanceof ArrowToken) {
			ArrowToken head = (ArrowToken) copy.head();
			this.hasSideEffects = !head.is_functional;
			copy.consume(1);
		} else {
			throw new Exception("expected arrow here");
		}

		this.returnType = new TypeNode(copy);

		copy.expectAndConsumeOtherWiseThrowException(new LCurlyToken());

		IToken statement_next = copy.get(0);
		while (!(statement_next instanceof RCurlyToken)) {
			this.statements.add(new StatementNode(copy));
			statement_next = copy.get(0);
		}

		copy.expectAndConsumeOtherWiseThrowException(new RCurlyToken());

		tokens.set(copy);
	}

};