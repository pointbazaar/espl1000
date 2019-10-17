package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;

import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.AssignmentStatementTypeChecker.typeCheckAssignmentStatementNode;
import static org.vanautrui.languages.compiler.typechecking.IfStatementNodeTypeChecker.typeCheckIfStatementNode;
import static org.vanautrui.languages.compiler.typechecking.LoopStatementNodeTypeChecker.typeCheckLoopStatementNode;
import static org.vanautrui.languages.compiler.typechecking.MethodCallNodeTypeChecker.typeCheckMethodCallNode;
import static org.vanautrui.languages.compiler.typechecking.ReturnStatementTypeChecker.typeCheckReturnStatementNode;
import static org.vanautrui.languages.compiler.typechecking.WhileStatementNodeTypeChecker.typeCheckWhileStatementNode;

public class StatementNodeTypeChecker {


  static void typeCheckStatementNode(List<AST> asts, NamespaceNode namespaceNode, MethodNode methodNode, StatementNode statementNode, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable) throws Exception {
    //it depends on the instance
    if (statementNode.statementNode instanceof AssignmentStatementNode) {
      AssignmentStatementNode assignmentStatementNode = (AssignmentStatementNode) statementNode.statementNode;
      typeCheckAssignmentStatementNode(asts, namespaceNode, methodNode, assignmentStatementNode, subTable, varTable);
    } else if (statementNode.statementNode instanceof LoopStatementNode) {
      LoopStatementNode loopStatementNode = (LoopStatementNode) statementNode.statementNode;
      typeCheckLoopStatementNode(asts, namespaceNode, methodNode, loopStatementNode, subTable, varTable);
    } else if (statementNode.statementNode instanceof WhileStatementNode) {
      WhileStatementNode whileStatementNode = (WhileStatementNode) statementNode.statementNode;
      typeCheckWhileStatementNode(asts, namespaceNode, methodNode, whileStatementNode, subTable, varTable);
    } else if (statementNode.statementNode instanceof MethodCallNode) {
      MethodCallNode methodCallNode = (MethodCallNode) statementNode.statementNode;
      typeCheckMethodCallNode(asts, namespaceNode, methodNode, methodCallNode, subTable, varTable);
    } else if (statementNode.statementNode instanceof IfStatementNode) {
      IfStatementNode ifStatementNode = (IfStatementNode) statementNode.statementNode;
      typeCheckIfStatementNode(asts, namespaceNode, methodNode, ifStatementNode, subTable, varTable);
    } else if (statementNode.statementNode instanceof ReturnStatementNode) {
      ReturnStatementNode returnStatementNode = (ReturnStatementNode) statementNode.statementNode;
      typeCheckReturnStatementNode(asts, namespaceNode, methodNode, returnStatementNode, subTable, varTable);
    } else {
      throw new Exception("unconsidered case in typechecking ");
    }
  }
}
