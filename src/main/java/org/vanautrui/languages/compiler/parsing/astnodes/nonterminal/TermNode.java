package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.IExpressionComputable;
import org.vanautrui.languages.compiler.parsing.astnodes.ITermNode;

public final class TermNode implements IASTNode, IExpressionComputable {

	public ITermNode termNode;

	public TermNode(final ITermNode itnode) {
		this.termNode = itnode;
	}

	@Override
	public String toSourceCode() {
		return termNode.toSourceCode();
	}
}
