package org.vanautrui.languages.compiler.simplify;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.OperatorNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.TermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.IntConstNode;

public final class SimplifierTest {

	@Test
	public void testSimpleSimplification(){

		/*
		1 + 2 -> 3
		 */

		final ExpressionNode expr = new ExpressionNode(
				new TermNode(new IntConstNode(1)),
				new OperatorNode("+"),
				new TermNode(new IntConstNode(2))
		);

		final ExpressionNode expr2 = Simplifier.simplifyExpressionNode(expr,false);
		
		//DEBUG
		//System.out.println(expr2.toSourceCode());

		Assert.assertTrue(expr2.term1.termNode instanceof IntConstNode);
		IntConstNode iconst = (IntConstNode)expr2.term1.termNode;
		Assert.assertEquals(3,iconst.number);

		Assert.assertEquals(true,expr2.term2.isEmpty());
		Assert.assertEquals(true,expr2.op.isEmpty());

	}
}
