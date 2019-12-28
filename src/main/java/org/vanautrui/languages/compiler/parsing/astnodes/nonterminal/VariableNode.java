package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.ITermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.SimpleVariableNode;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public final class VariableNode implements IASTNode, ITermNode {

	public SimpleVariableNode simpleVariableNode;
	public List<VariableNode> memberAccessList = new ArrayList<>();

	public VariableNode(){}

	public VariableNode(SimpleVariableNode simpleVariableNode, List<VariableNode> list) {
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
