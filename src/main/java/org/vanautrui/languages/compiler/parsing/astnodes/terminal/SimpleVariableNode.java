package org.vanautrui.languages.compiler.parsing.astnodes.terminal;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;

import java.util.Optional;

public final class SimpleVariableNode implements IASTNode {

	//this identifies a variable

	public final String name;

	public final Optional<ExpressionNode> indexOptional;

	public SimpleVariableNode(final String name) {
		this.name = name;
		this.indexOptional = Optional.empty();
	}

	@Override
	public String toSourceCode() {
		return this.indexOptional
				.map(expressionNode -> this.name + "[" + expressionNode.toSourceCode() + "]")
				.orElse(this.name);
	}

}
