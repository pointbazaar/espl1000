package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.ITermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.SimpleVariableNode;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public final class VariableNode implements IASTNode, ITermNode {

	public final SimpleVariableNode simpleVariableNode;
	public final List<SimpleVariableNode> memberAccessList = new ArrayList<>();

	public VariableNode(SimpleVariableNode simpleVariableNode, List<SimpleVariableNode> list) {
		this.simpleVariableNode = simpleVariableNode;
		this.memberAccessList.addAll(list);
	}

	public VariableNode(SimpleVariableNode simpleVariableNode) {
		this.simpleVariableNode = simpleVariableNode;
	}

	public VariableNode(VariableNode variableNode) {
		this.simpleVariableNode = variableNode.simpleVariableNode;
		this.memberAccessList.addAll(variableNode.memberAccessList);
	}

	@Override
	public String toSourceCode() {
		return this.simpleVariableNode.toSourceCode()
				+ this.memberAccessList
				.stream()
				.map(ma -> "." + ma.toSourceCode())
				.collect(Collectors.joining(""));
	}
}
