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

		Assert.assertTrue(expr2.term2.isEmpty());
		Assert.assertTrue(expr2.op.isEmpty());

	}

	@Test
	public void testSimplificationOfNestedExpression(){

		/*
		1 + (2+3) -> 6
		 */

		final ExpressionNode expr = new ExpressionNode(
				new TermNode(new IntConstNode(1)),
				new OperatorNode("+"),
				new TermNode(
						new ExpressionNode(
								new TermNode(new IntConstNode(2)),
								new OperatorNode("+"),
								new TermNode(new IntConstNode(3))
						)
				)
		);

		//DEBUG
		//System.out.println(expr.toSourceCode());

		final ExpressionNode expr2 = Simplifier.simplifyExpressionNode(expr,false);

		//DEBUG
		//System.out.println(expr2.toSourceCode());

		Assert.assertTrue(expr2.term1.termNode instanceof IntConstNode);
		IntConstNode iconst = (IntConstNode)expr2.term1.termNode;
		Assert.assertEquals(6,iconst.number);

		Assert.assertTrue(expr2.term2.isEmpty());
		Assert.assertTrue(expr2.op.isEmpty());

	}

	@Test
	public void testSimplificationComplicated(){

		/*
		1 + ( 2 * ( 3 % 2) ) / 7 -> 1 + ( 2 ) / 7 -> 1
		1 + (...)
			(2 * (...)) / 7
		 */

		final ExpressionNode expr = new ExpressionNode(
				new TermNode(new IntConstNode(1)),
				new OperatorNode("+"),
				new TermNode(
						new ExpressionNode(
								new TermNode(new ExpressionNode(
										new TermNode(new IntConstNode(2)),
										new OperatorNode("*"),
										new TermNode(new ExpressionNode(
												new TermNode(new IntConstNode(3)),
												new OperatorNode("%"),
												new TermNode(new IntConstNode(2))
										))
								)),
								new OperatorNode("/"),
								new TermNode(new IntConstNode(7))
						)
				)
		);

		//DEBUG
		//System.out.println(expr.toSourceCode());

		final ExpressionNode expr2 = Simplifier.simplifyExpressionNode(expr,false);

		//DEBUG
		//System.out.println(expr2.toSourceCode());

		Assert.assertTrue(expr2.term1.termNode instanceof IntConstNode);
		IntConstNode iconst = (IntConstNode)expr2.term1.termNode;
		Assert.assertEquals(1,iconst.number);

		Assert.assertTrue(expr2.term2.isEmpty());
		Assert.assertTrue(expr2.op.isEmpty());

	}
}
