package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements;

import org.vanautrui.languages.compiler.parsing.IASTNode;

public final class StatementNode implements IASTNode {

	//can be method call ,loop statement, while statement, ...
	//TODO: figure out  how to deserialize it with jackson
	public IStatementNode statementNode;

	public StatementNode(){}

	public StatementNode(final IStatementNode stmt) {
		this.statementNode = stmt;
	}


	@Override
	public String toSourceCode() {
		return this.statementNode.toSourceCode()
				+ ((statementNode instanceof MethodCallNode) ? ";" : "");
	}

	public boolean containsSubroutineCalls(){
		return this.statementNode.containsSubroutineCalls();
	}
}
