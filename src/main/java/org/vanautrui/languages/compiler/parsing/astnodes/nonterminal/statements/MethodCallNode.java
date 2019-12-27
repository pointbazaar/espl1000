package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.IExpressionComputable;
import org.vanautrui.languages.compiler.parsing.astnodes.ITermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public final class MethodCallNode implements IASTNode, IStatementNode, IExpressionComputable, ITermNode {

	public final String methodName;

	public final List<ExpressionNode> argumentList = new ArrayList<>();

	public MethodCallNode(final String methodName, final List<ExpressionNode> argumentList) {
		this.methodName = methodName;
		this.argumentList.addAll(argumentList);
	}

	@Override
	public String toSourceCode() {
		return this.methodName
				+ "(" + this.argumentList.stream().map(
				ExpressionNode::toSourceCode
		).collect(Collectors.joining(","))
				+ ")";
	}

}
