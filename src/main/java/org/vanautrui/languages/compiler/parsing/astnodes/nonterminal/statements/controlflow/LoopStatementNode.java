package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.IStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;

import java.util.List;

public final class LoopStatementNode implements IASTNode, IStatementNode {

	public ExpressionNode count;

	public List<StatementNode> statements;

	public LoopStatementNode(){}

	public LoopStatementNode(final ExpressionNode count, final List<StatementNode> statements) {

		this.count=count;
		this.statements=statements;
	}

	@Override
	public String toSourceCode() {
		return " loop " + this.count.toSourceCode() + " { " + this.statements.stream().map(stmt -> stmt.toSourceCode()) + " } ";
	}

	@Override
	public boolean containsSubroutineCalls() {
		return count.containsSubroutineCalls() || statements.stream().anyMatch(StatementNode::containsSubroutineCalls);
	}
}
