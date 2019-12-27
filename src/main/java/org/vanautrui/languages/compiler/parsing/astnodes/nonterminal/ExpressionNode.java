package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.ITermNode;

import java.util.ArrayList;
import java.util.List;

public final class ExpressionNode implements IASTNode, ITermNode {

	//DragonExpressionNode should be similar to jack expression
	//an expression should be anything that returns a value or computes to a value

	public TermNode term;

	public List<OperatorNode> operatorNodes;

	public List<TermNode> termNodes;

	public ExpressionNode(){}

	public ExpressionNode(final TermNode term) {
		this.term = term;
		this.operatorNodes=new ArrayList<>();
		this.termNodes=new ArrayList<>();
	}

	public ExpressionNode(final TermNode term, final List<OperatorNode> operators, final List<TermNode> termNodes){
		this.term=term;
		this.operatorNodes=operators;
		this.termNodes=termNodes;
	}

	@Override
	public String toSourceCode() {
		final StringBuilder res = new StringBuilder(term.toSourceCode());

		for (int i = 0; i < this.operatorNodes.size(); i++) {
			OperatorNode o = this.operatorNodes.get(i);
			TermNode t = this.termNodes.get(i);
			res.append(" ").append(o.toSourceCode()).append(" ").append(t.toSourceCode()).append(" ");
		}

		return res.toString();
	}
}
