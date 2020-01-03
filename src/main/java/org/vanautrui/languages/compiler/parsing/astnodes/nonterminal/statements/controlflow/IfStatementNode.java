package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.IStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;

import java.util.List;
import java.util.stream.Collectors;

public final class IfStatementNode implements IASTNode, IStatementNode {

	public ExpressionNode condition;

	public List<StatementNode> statements;

	public List<StatementNode> elseStatements;

	public IfStatementNode(){}

	public IfStatementNode(final ExpressionNode condition, final List<StatementNode> statements, final List<StatementNode> elseStatements) {
		this.condition=condition;
		this.statements=statements;
		this.elseStatements=elseStatements;
	}

	@Override
	public String toSourceCode() {
		final String if_string = " if ( " + this.condition.toSourceCode() + " ) { " + this.statements.stream().map(stmt -> stmt.toSourceCode()).collect(Collectors.joining(" ")) + " } ";

		final String else_string = " else {" + this.elseStatements.stream().map(stmt -> stmt.toSourceCode()).collect(Collectors.joining(" ")) + " } ";
		if (elseStatements.size() > 0) {
			return if_string + else_string;
		} else {
			return if_string;
		}
	}

	@Override
	public boolean containsSubroutineCalls() {
		return this.condition.containsSubroutineCalls() ||
		       this.statements.stream().anyMatch(StatementNode::containsSubroutineCalls)
				|| this.elseStatements.stream().anyMatch(StatementNode::containsSubroutineCalls);
	}
}
