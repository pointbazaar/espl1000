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
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;

import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.AssignmentStatementTypeChecker.typeCheckAssignmentStatementNode;
import static org.vanautrui.languages.compiler.typechecking.IfStatementNodeTypeChecker.typeCheckIfStatementNode;
import static org.vanautrui.languages.compiler.typechecking.LoopStatementNodeTypeChecker.typeCheckLoopStatementNode;
import static org.vanautrui.languages.compiler.typechecking.MethodCallNodeTypeChecker.typeCheckMethodCallNode;
import static org.vanautrui.languages.compiler.typechecking.ReturnStatementTypeChecker.typeCheckReturnStatementNode;
import static org.vanautrui.languages.compiler.typechecking.WhileStatementNodeTypeChecker.typeCheckWhileStatementNode;

public class StatementNodeTypeChecker {


    static void typeCheckStatementNode(
            List<AST> asts,
            NamespaceNode namespace,
            MethodNode methodNode,
            StatementNode node,
            SubroutineSymbolTable subTable,
            LocalVarSymbolTable varTable,
            StructsSymbolTable structsTable
    ) throws Exception {
        //it depends on the instance
        if (node.statementNode instanceof AssignmentStatementNode) {
            AssignmentStatementNode assignmentStatementNode = (AssignmentStatementNode) node.statementNode;
            typeCheckAssignmentStatementNode(asts, namespace, methodNode, assignmentStatementNode, subTable, varTable,structsTable);
        } else if (node.statementNode instanceof LoopStatementNode) {
            LoopStatementNode loopStatementNode = (LoopStatementNode) node.statementNode;
            typeCheckLoopStatementNode(asts, namespace, methodNode, loopStatementNode, subTable, varTable,structsTable);
        } else if (node.statementNode instanceof WhileStatementNode) {
            WhileStatementNode whileStatementNode = (WhileStatementNode) node.statementNode;
            typeCheckWhileStatementNode(asts, namespace, methodNode, whileStatementNode, subTable, varTable,structsTable);
        } else if (node.statementNode instanceof MethodCallNode) {
            MethodCallNode methodCallNode = (MethodCallNode) node.statementNode;
            typeCheckMethodCallNode(asts, namespace, methodNode, methodCallNode, subTable, varTable,structsTable);
        } else if (node.statementNode instanceof IfStatementNode) {
            IfStatementNode ifStatementNode = (IfStatementNode) node.statementNode;
            typeCheckIfStatementNode(asts, namespace, methodNode, ifStatementNode, subTable, varTable,structsTable);
        } else if (node.statementNode instanceof ReturnStatementNode) {
            ReturnStatementNode returnStatementNode = (ReturnStatementNode) node.statementNode;
            typeCheckReturnStatementNode(asts, namespace, methodNode, returnStatementNode, subTable, varTable,structsTable);
        } else {
            throw new Exception("unconsidered case in typechecking ");
        }
    }
}
