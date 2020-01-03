package org.vanautrui.languages.compiler.simplify;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.IStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.BoolConstNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.IntConstNode;

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
		if(debug){
			System.out.println("Eliminator::isRequiredIfStatement");
		}

		//if the condition is false then it is not required,
		//unless there are subroutine calls somewhere in there
		if(!istmt.containsSubroutineCalls()){
			//contains no subroutine calls

			if(istmt.statements.size()==0 && istmt.elseStatements.size()==0){
				return false;
			}

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
			return isRequiredLoopStatement((LoopStatementNode)istmt,debug);
		}else if(istmt instanceof WhileStatementNode) {
			return isRequiredWhileStatement((WhileStatementNode)istmt,debug);
		}else if(istmt instanceof AssignmentStatementNode) {
			//Maybe we could assign to a value that is not used afterwards.
			//However, this needs more advanced analysis of the context of a statement.
			//This may be done in later phases of implementation.
			return true;
		}else if(istmt instanceof MethodCallNode) {
			//subroutine calls are always required, until we start
			//to look at which of these have side effects and which do not.
			//this could be done in later phases of implementation
			return true;
		}else {
			throw new RuntimeException("Fatality");
		}
	}

	private static boolean isRequiredWhileStatement(
			final WhileStatementNode wstmt,
			final boolean debug
	) {
		if(debug){
			System.out.println("Eliminator::isRequiredWhileStatement");
		}
		if(!wstmt.containsSubroutineCalls()) {
			//consider an empty while statement. it should not be required
			if(wstmt.statements.size()==0){
				return false;
			}

			//a while statement with a condition of 'false' should not be required
			final ExpressionNode condition = wstmt.condition;
			if(
				condition.term1.termNode instanceof BoolConstNode
				&& ((BoolConstNode)condition.term1.termNode).boolValue==false
				&& condition.term2.isEmpty()
			){
				return false;
			}
		}
		return true;
	}

	private static boolean isRequiredLoopStatement(
			final LoopStatementNode lstmt,
			final boolean debug
	) {
		if(debug){
			System.out.println("Eliminator::isRequiredLoopStatement");
		}

		if (!lstmt.containsSubroutineCalls()) {
			if(lstmt.statements.size()==0){
				return false;
			}

			final ExpressionNode count = lstmt.count;
			//check if the loop count has been simplified to 0
			if (count.term1.termNode instanceof IntConstNode
			    && count.term2.isEmpty()
			    && ((IntConstNode) count.term1.termNode).number == 0
			) {
				return false;
			}
		}

		return true;
	}
}
