package org.vanautrui.languages.compiler.simplify;

import org.junit.Assert;
import org.junit.Ignore;
import org.junit.Test;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.OperatorNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.TermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.VariableNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.BoolConstNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.IntConstNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.SimpleVariableNode;

public final class ExpressionSimplifierTest {

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

		final ExpressionNode expr2 = ExpressionSimplifier.simplifyExpressionNode(expr,false);
		
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

		final ExpressionNode expr2 = ExpressionSimplifier.simplifyExpressionNode(expr,false);

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

		final ExpressionNode expr2 = ExpressionSimplifier.simplifyExpressionNode(expr,false);

		//DEBUG
		//System.out.println(expr2.toSourceCode());

		Assert.assertTrue(expr2.term1.termNode instanceof IntConstNode);
		IntConstNode iconst = (IntConstNode)expr2.term1.termNode;
		Assert.assertEquals(1,iconst.number);

		Assert.assertTrue(expr2.term2.isEmpty());
		Assert.assertTrue(expr2.op.isEmpty());

	}

	@Test
	public void testSimplificationOfComparisons(){

		/*
		1 < 2 -> true
		 */

		final ExpressionNode expr = new ExpressionNode(
				new TermNode(new IntConstNode(1)),
				new OperatorNode("<"),
				new TermNode(new IntConstNode(2))
		);

		final ExpressionNode expr2 = ExpressionSimplifier.simplifyExpressionNode(expr,false);

		//DEBUG
		//System.out.println(expr2.toSourceCode());

		Assert.assertTrue(expr2.term1.termNode instanceof BoolConstNode);
		BoolConstNode iconst = (BoolConstNode) expr2.term1.termNode;
		Assert.assertTrue(iconst.boolValue);

		Assert.assertTrue(expr2.term2.isEmpty());
		Assert.assertTrue(expr2.op.isEmpty());

	}

	@Ignore
	@Test
	public void testCanLiftUnknownToPerformSimplification(){

		/*
		3 + (1 + a) -> 4 + a
		 */

		final ExpressionNode expr = new ExpressionNode(
				new TermNode(new IntConstNode(3)),
				new OperatorNode("+"),
				new TermNode(
			new ExpressionNode(
						new TermNode(new IntConstNode(1)),
						new OperatorNode("+"),
						new TermNode(new VariableNode(new SimpleVariableNode("a")))
					)
				)
		);

		//DEBUG
		System.out.println(expr.toSourceCode());

		final ExpressionNode expr2 = ExpressionSimplifier.simplifyExpressionNode(expr,false);

		//DEBUG
		System.out.println(expr2.toSourceCode());

		Assert.assertTrue(expr2.term1.termNode instanceof IntConstNode);
		IntConstNode iconst = (IntConstNode) expr2.term1.termNode;
		Assert.assertEquals(4,iconst.number);

		Assert.assertTrue(expr2.op.isPresent());
		Assert.assertEquals("+",expr2.op.get());

		Assert.assertTrue(expr2.term2.isPresent());
		final TermNode term2 = expr2.term2.get();
		final VariableNode v = (VariableNode)term2.termNode;
		final SimpleVariableNode sv = v.simpleVariableNode;
		Assert.assertEquals("a",sv.name);

	}
}
