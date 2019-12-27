package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal;

import org.vanautrui.languages.compiler.parsing.IASTNode;

public final class OperatorNode implements IASTNode {

	public String operator;

	public OperatorNode(){}

	public OperatorNode(final String op){
		this.operator=op;
	}

	@Override
	public String toSourceCode() {
		return " " + operator + " ";
	}
}
