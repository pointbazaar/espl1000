package org.vanautrui.languages.compiler.simplify;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.OperatorNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.TermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.IStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.BoolConstNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.FloatConstNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.IConstantNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.IntConstNode;

import java.util.stream.Collectors;

public final class Simplifier {

	public static NamespaceNode simplifyNamespaceNode(
			final NamespaceNode namespaceNode,
			final boolean debug
	){
		if(debug){
			System.out.println("Simplifier::simplifyNamespaceNode");
		}

		final NamespaceNode result =  new NamespaceNode();
		result.structs = namespaceNode.structs;
		result.methods = namespaceNode.methods.stream().map((MethodNode subr) -> simplifyMethodNode(subr,debug)).collect(Collectors.toList());;
		return result;
	}

	public static MethodNode simplifyMethodNode(final MethodNode subr,final boolean debug){

		subr.statements = subr.statements
				.stream()
				.map(stmt-> simplifyStatementNode(stmt,debug))
				.collect(Collectors.toList());

		return subr;
	}

	public static StatementNode simplifyStatementNode(final StatementNode stmt,final boolean debug){

		final IStatementNode istmt = stmt.statementNode;

		if(istmt instanceof AssignmentStatementNode){
			return new StatementNode(simplifyAssignmentStatementNode((AssignmentStatementNode)istmt,debug));
		}else if(istmt instanceof IfStatementNode){
			//TODO: see if the condition is false, then the statements in the 'if' block can be removed,
			//TODO: and the statements in the 'else' block can be lifted out.
			return new StatementNode(simplifyIfStatementNode((IfStatementNode)istmt,debug));
		}else if(istmt instanceof LoopStatementNode){
			//TODO: see if the loop only executes once, maybe we can lift the statements out of it
			//TODO: see if the loop doesn't execute at all, then we can get rid of these statements
			return new StatementNode(simplifyLoopStatementNode((LoopStatementNode)istmt,debug));
		}else if(istmt instanceof WhileStatementNode){
			//TODO: see if the condition is false, then we can get rid of all the statements in the while loop
			return new StatementNode(simplifyWhileStatementNode((WhileStatementNode)istmt,debug));
		}else if(istmt instanceof ReturnStatementNode){
			return new StatementNode(simplifyReturnStatementNode((ReturnStatementNode)istmt,debug));
		}else if(istmt instanceof MethodCallNode){
			return new StatementNode(simplifyMethodCallNode((MethodCallNode)istmt,debug));
		}

		return new StatementNode(istmt);
	}

	private static LoopStatementNode simplifyLoopStatementNode(LoopStatementNode lstmt, boolean debug) {
		return new LoopStatementNode(
			simplifyExpressionNode(lstmt.count,debug),
			lstmt.statements
					.stream()
					.map(stmt->simplifyStatementNode(stmt,debug))
					.collect(Collectors.toList())
		);
	}

	private static IStatementNode simplifyIfStatementNode(IfStatementNode ifstmt, boolean debug) {
		return new IfStatementNode(
				simplifyExpressionNode(ifstmt.condition,debug),
				ifstmt.statements
						.stream()
						.map(stmt->simplifyStatementNode(stmt,debug))
						.collect(Collectors.toList()),
				ifstmt.elseStatements
						.stream()
						.map(stmt->simplifyStatementNode(stmt,debug))
						.collect(Collectors.toList())
		);
	}

	private static WhileStatementNode simplifyWhileStatementNode(
			final WhileStatementNode wstmt,
			final boolean debug
	) {
		return new WhileStatementNode(
				simplifyExpressionNode(wstmt.condition,debug),
				wstmt.statements
						.stream()
						.map(stmt->simplifyStatementNode(stmt,debug))
						.collect(Collectors.toList())
		);
	}

	private static MethodCallNode simplifyMethodCallNode(
			final MethodCallNode mCall,
			final boolean debug
	) {
		return new MethodCallNode(
				mCall.methodName,
				mCall.arguments
						.stream()
						.map(arg->simplifyExpressionNode(arg,debug))
						.collect(Collectors.toList())
		);
	}

	private static ReturnStatementNode simplifyReturnStatementNode(ReturnStatementNode rstmt, boolean debug) {
		return new ReturnStatementNode(simplifyExpressionNode(rstmt.returnValue,debug));
	}

	public static AssignmentStatementNode simplifyAssignmentStatementNode(final AssignmentStatementNode astmt,final boolean debug){
		return new AssignmentStatementNode(astmt.variableNode,simplifyExpressionNode(astmt.expressionNode,debug));
	}

	public static TermNode mayUnwrapExpression(final ExpressionNode expr,final boolean debug)throws Exception{
		//TODO: use this subroutine where it makes sense
		if(expr.op.isEmpty()){
			return expr.term1;
		}
		throw new Exception("could not unwrap.");
	}

	public static ExpressionNode simplifyExpressionNode(final ExpressionNode expr,final boolean debug){

		if(expr.op.isEmpty()){
			//nothing to do, as there is just 2 expression
			return new ExpressionNode(simplifyTermNode(expr.term1,debug));
		}

		//op and term2 should be there
		final TermNode term1 = expr.term1;
		final OperatorNode op = expr.op.get();
		final TermNode term2 = expr.term2.get();

		return simplifyExpressionInner(term1,op,term2,debug);
	}

	private static TermNode simplifyTermNode(TermNode term1,final boolean debug) {

		if(term1.termNode instanceof ExpressionNode){
			return new TermNode(simplifyExpressionNode((ExpressionNode)term1.termNode,debug));
		}
		return term1;
	}

	public static ExpressionNode simplifyExpressionInner(
			final TermNode term1,
			final OperatorNode op,
			final TermNode term2,
			final boolean debug
	){
		//this is in the case we have expression with 2 terms
		//TODO: there should be a binary tree where each of the 2 leaves is guaranteed to exist
		//TODO: this would make these routines much simpler

		//try to simplify terms in case there is a subexpression among them that can be simplified.
		TermNode nTerm1 = simplifyTermNode(term1,debug);
		TermNode nTerm2 = simplifyTermNode(term2,debug);

		//TODO: maybe lift a term from expression in term. this could be the cause for the failing test
		if(nTerm1.termNode instanceof ExpressionNode){
			try {
				nTerm1 = mayUnwrapExpression((ExpressionNode)nTerm1.termNode,debug);
			} catch (Exception e) {
				//pass
			}
		}

		if(nTerm2.termNode instanceof ExpressionNode){
			try {
				nTerm2 = mayUnwrapExpression((ExpressionNode)nTerm2.termNode,debug);
			} catch (Exception e) {
				//pass
			}
		}

		//if we then have 2 constant expressions, we can simplify it.
		if(nTerm1.termNode instanceof IConstantNode && nTerm2.termNode instanceof IConstantNode){
			return new ExpressionNode(evaluateConstantExpression(
					(IConstantNode)nTerm1.termNode,
					op,
					(IConstantNode)nTerm2.termNode,
					debug
			));
		}else{
			return new ExpressionNode(nTerm1,op,nTerm2);
		}
	}

	private static TermNode evaluateConstantExpression(IConstantNode term1, OperatorNode op, IConstantNode term2, boolean debug) {
		//typechecking has already happened, so
		//we do not need to confirm the types here again.

		if(term1 instanceof IntConstNode
			&& term2 instanceof IntConstNode
		){
			final IntConstNode i1 = (IntConstNode)term1;
			final IntConstNode i2 = (IntConstNode)term2;
			switch (op.operator){
				//arithmetic
				case "+":
					return new TermNode(new IntConstNode(i1.number + i2.number));
				case "-":
					return new TermNode(new IntConstNode(i1.number - i2.number));
				case "*":
					return new TermNode(new IntConstNode(i1.number * i2.number));
				case "/":
					return new TermNode(new IntConstNode(i1.number / i2.number));
				case "%":
					return new TermNode(new IntConstNode(i1.number % i2.number));

				//comparisons
				case ">":
					return new TermNode(new BoolConstNode(i1.number > i2.number));
				case ">=":
					return new TermNode(new BoolConstNode(i1.number >= i2.number));
				case "<":
					return new TermNode(new BoolConstNode(i1.number < i2.number));
				case "<=":
					return new TermNode(new BoolConstNode(i1.number <= i2.number));
				case "!=":
					return new TermNode(new BoolConstNode(i1.number != i2.number));
				case "==":
					return new TermNode(new BoolConstNode(i1.number == i2.number));
			}

		}else if(
				term1 instanceof FloatConstNode
				&& term2 instanceof FloatConstNode
		){
			final FloatConstNode i1 = (FloatConstNode)term1;
			final FloatConstNode i2 = (FloatConstNode)term2;
			switch (op.operator){
				//arithmetic
				case "+":
					return new TermNode(new FloatConstNode(i1.floatValue + i2.floatValue));
				case "-":
					return new TermNode(new FloatConstNode(i1.floatValue - i2.floatValue));
				case "*":
					return new TermNode(new FloatConstNode(i1.floatValue * i2.floatValue));
				case "/":
					return new TermNode(new FloatConstNode(i1.floatValue / i2.floatValue));
				case "%":
					return new TermNode(new FloatConstNode(i1.floatValue % i2.floatValue));

				//comparisons
				case ">":
					return new TermNode(new BoolConstNode(i1.floatValue > i2.floatValue));
				case ">=":
					return new TermNode(new BoolConstNode(i1.floatValue >= i2.floatValue));
				case "<":
					return new TermNode(new BoolConstNode(i1.floatValue < i2.floatValue));
				case "<=":
					return new TermNode(new BoolConstNode(i1.floatValue <= i2.floatValue));
				case "!=":
					return new TermNode(new BoolConstNode(i1.floatValue != i2.floatValue));
				case "==":
					return new TermNode(new BoolConstNode(i1.floatValue == i2.floatValue));
			}
		}else if(
				term1 instanceof BoolConstNode
				&& term2 instanceof BoolConstNode
		){
			final BoolConstNode i1 = (BoolConstNode)term1;
			final BoolConstNode i2 = (BoolConstNode)term2;
			switch (op.operator){
				case "&&":
					return new TermNode(new BoolConstNode(i1.boolValue && i2.boolValue));
				case "||":
					return new TermNode(new BoolConstNode(i1.boolValue || i2.boolValue));
				case "!=":
					return new TermNode(new BoolConstNode(i1.boolValue != i2.boolValue));
				case "==":
					return new TermNode(new BoolConstNode(i1.boolValue == i2.boolValue));
			}
		}

		throw new RuntimeException("fatality");
	}
}
