package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.VariableNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;

import java.util.Optional;

public final class AssignmentStatementNode implements IASTNode, IStatementNode {

	public Optional<TypeNode> optTypeNode;

	public VariableNode variableNode;

	public ExpressionNode expressionNode;

	public AssignmentStatementNode(){}

	public AssignmentStatementNode(final VariableNode var, final ExpressionNode expr) {
		this.variableNode = var;
		this.expressionNode = expr;
		this.optTypeNode = Optional.empty();
	}

	private String getStringForTypeOrEmptyString() {
		return this.optTypeNode
				.map(typeNode -> typeNode.toSourceCode() + " ")
				.orElse("");
	}

	@Override
	public String toSourceCode() {
		return this.getStringForTypeOrEmptyString() + variableNode.toSourceCode()
				+ " = " + this.expressionNode.toSourceCode() + ";";
	}
}
