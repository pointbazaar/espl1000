package org.vanautrui.languages.compiler.simplify;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.IStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.BoolConstNode;

public final class Eliminator {
	/*
	After the simplifier has simplified various Nodes in the AST,
	the Eliminator tells us if we can eliminate them.

	This requires the Symbol Tables in case there are subroutine calls in there.
	As these subroutines could have side effects.
	If there are, then we need to keep these calls for their side effects.

	For now, we just simplify the task, and say that any and all method calls
	are required.
	 */

	public static boolean isRequiredIfStatement(
			final IfStatementNode istmt,
			final boolean debug
	){
		//if the condition is false then it is not required,
		//unless there are subroutine calls somewhere in there
		if(istmt.containsSubroutineCalls()){
			return true;
		}else{
			//contains no subroutine calls
			if(istmt.condition.term1.termNode instanceof BoolConstNode
			   && (((BoolConstNode)istmt.condition.term1.termNode).boolValue == false)
				&& istmt.condition.term2.isEmpty()
					&& istmt.elseStatements.size()==0
			){
				return false;
			}
		}
		//TODO: consider the other cases

		return true;
	}

	public static boolean isRequired(
			final StatementNode stmt,
			final boolean debug
	) {
		final IStatementNode istmt = stmt.statementNode;

		if(istmt instanceof IfStatementNode){
			return isRequiredIfStatement((IfStatementNode)istmt,debug);
		}else if(istmt instanceof LoopStatementNode){
			//TODO
			return true;
		}else if(istmt instanceof WhileStatementNode) {
			//TODO
			return true;
		}else if(istmt instanceof AssignmentStatementNode) {
			//TODO
			return true;
		}else if(istmt instanceof MethodCallNode) {
			//TODO
			return true;
		}else {
			throw new RuntimeException("Fatality");
		}
	}
}
