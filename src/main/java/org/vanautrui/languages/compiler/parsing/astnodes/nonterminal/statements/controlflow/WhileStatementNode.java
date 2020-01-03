package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.IStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;

import java.util.List;
import java.util.stream.Collectors;

public final class WhileStatementNode implements IASTNode, IStatementNode {

	public ExpressionNode condition;

	public List<StatementNode> statements;

	public WhileStatementNode(){}

	public WhileStatementNode(final ExpressionNode condition, final List<StatementNode> statements) {

		this.condition=condition;
		this.statements=statements;
	}

	@Override
	public String toSourceCode() {
		return " while ( " + this.condition.toSourceCode() + " ) { " + this.statements.stream().map(stmt -> stmt.toSourceCode()).collect(Collectors.joining(" ")) + " } ";
	}

	@Override
	public boolean containsSubroutineCalls() {
		return condition.containsSubroutineCalls() || statements.stream().anyMatch(StatementNode::containsSubroutineCalls);
	}
}
