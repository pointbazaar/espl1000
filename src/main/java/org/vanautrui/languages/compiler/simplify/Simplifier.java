package org.vanautrui.languages.compiler.simplify;

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

import java.util.stream.Collectors;

import static org.vanautrui.languages.compiler.simplify.ExpressionSimplifier.simplifyExpressionNode;

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

}
