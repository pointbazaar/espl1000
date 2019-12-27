package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.ITermNode;

import java.util.List;
import java.util.stream.Collectors;

public final class ArrayConstantNode implements IASTNode, ITermNode {

	public List<ExpressionNode> elements;

	public ArrayConstantNode(final List<ExpressionNode> elements){
		this.elements=elements;
	}

	@Override
	public String toSourceCode() {
		return " [" + this.elements.stream().map(ExpressionNode::toSourceCode).collect(Collectors.joining(",")) + "] ";
	}
}
