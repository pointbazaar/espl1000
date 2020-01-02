package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.ITermNode;

import java.util.Optional;

public final class ExpressionNode implements IASTNode, ITermNode {

	//DragonExpressionNode should be similar to jack expression
	//an expression should be anything that returns a value or computes to a value

	public TermNode term1;
	public Optional<OperatorNode> op = Optional.empty();
	public Optional<TermNode> term2 = Optional.empty();

	public ExpressionNode(){}

	public ExpressionNode(final TermNode term1) {
		this.term1 = term1;
	}

	public ExpressionNode(final TermNode term1, final OperatorNode op, final TermNode term2){
		this.term1 = term1;
		this.op = Optional.of(op);
		this.term2 = Optional.of(term2);
	}

	@Override
	public String toSourceCode() {
		if(this.op.isPresent() && this.term2.isPresent()){

			return this.term1.toSourceCode()+" "+this.op.get().toSourceCode()+" "+this.term2.get().toSourceCode();
		}else{
			return this.term1.toSourceCode();
		}
	}
}
