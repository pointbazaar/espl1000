package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.ITermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;

public final class TermNode implements IASTNode {

	public ITermNode termNode;

	public TermNode(){}

	public TermNode(final ITermNode itnode) {
		this.termNode = itnode;
	}

	@Override
	public String toSourceCode() {
		return termNode.toSourceCode();
	}

	public boolean containsSubroutineCalls() {
		return termNode instanceof MethodCallNode;
	}
}
