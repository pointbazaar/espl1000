package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.IStatementNode;

public final class ReturnStatementNode implements IASTNode, IStatementNode {

	public final ExpressionNode returnValue;

	public ReturnStatementNode(final ExpressionNode returnValue) {
		this.returnValue = returnValue;
	}

	@Override
	public String toSourceCode() {
		return "return " + this.returnValue.toSourceCode() + ";";
	}
}
