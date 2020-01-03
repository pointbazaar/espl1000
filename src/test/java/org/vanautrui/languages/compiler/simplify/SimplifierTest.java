package org.vanautrui.languages.compiler.simplify;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.TermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.VariableNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.BoolConstNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.IntConstNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.SimpleVariableNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SimpleTypeNode;

import java.util.ArrayList;
import java.util.Arrays;

public final class SimplifierTest {

	@Test
	public void testCanEliminateIfStatement(){

		/*
		if(false){
			x=3;
		}
		-> //nothing at all
		 */

		final MethodNode m = new MethodNode(
				new TypeNode(new SimpleTypeNode("Int")),
				"main",
				Arrays.asList(new StatementNode(
						new IfStatementNode(
								new ExpressionNode(new TermNode(new BoolConstNode(false))),
								Arrays.asList(
										new StatementNode(
												new AssignmentStatementNode(
														new VariableNode(new SimpleVariableNode("x")),
														new ExpressionNode(new TermNode(new IntConstNode(3)))
												)
										)
								),
								new ArrayList<>()
						)
				))
		);

		//DEBUG
		//System.out.println(m.toSourceCode());

		final MethodNode m2 = Simplifier.simplifyMethodNode(m,false);

		//DEBUG
		//System.out.println(m2.toSourceCode());

		Assert.assertEquals(0,m2.statements.size());
	}
}
