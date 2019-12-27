package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal;

import org.vanautrui.languages.compiler.parsing.IASTNode;

public final class OperatorNode implements IASTNode {

	public final String operator;

	public OperatorNode(final String op){
		this.operator=op;
	}

	@Override
	public String toSourceCode() {
		return " " + operator + " ";
	}
}
