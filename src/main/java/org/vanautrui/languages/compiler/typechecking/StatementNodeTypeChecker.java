package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;

import static org.vanautrui.languages.compiler.typechecking.AssignmentStatementTypeChecker.typeCheckAssignmentStatementNode;
import static org.vanautrui.languages.compiler.typechecking.IfStatementNodeTypeChecker.typeCheckIfStatementNode;
import static org.vanautrui.languages.compiler.typechecking.LoopStatementNodeTypeChecker.typeCheckLoopStatementNode;
import static org.vanautrui.languages.compiler.typechecking.MethodCallNodeTypeChecker.typeCheckMethodCallNode;
import static org.vanautrui.languages.compiler.typechecking.ReturnStatementTypeChecker.typeCheckReturnStatementNode;
import static org.vanautrui.languages.compiler.typechecking.WhileStatementNodeTypeChecker.typeCheckWhileStatementNode;

public final class StatementNodeTypeChecker {

    static void typeCheckStatementNode(
            final AST_Whole_Program asts,
            final NamespaceNode namespace,
            final MethodNode methodNode,
            final StatementNode node,
            final SymbolTableContext ctx
    ) throws Exception {

        final SubroutineSymbolTable subTable = ctx.subTable;
        final LocalVarSymbolTable varTable = ctx.varTable;
        final StructsSymbolTable structsTable = ctx.structsTable;

        //it depends on the instance
        if (node.statementNode instanceof AssignmentStatementNode) {
            final AssignmentStatementNode assignmentStatementNode = (AssignmentStatementNode) node.statementNode;
            typeCheckAssignmentStatementNode(asts, namespace, methodNode, assignmentStatementNode, ctx);
        } else if (node.statementNode instanceof LoopStatementNode) {
            final LoopStatementNode loopStatementNode = (LoopStatementNode) node.statementNode;
            typeCheckLoopStatementNode(asts, namespace, methodNode, loopStatementNode, ctx);
        } else if (node.statementNode instanceof WhileStatementNode) {
            final WhileStatementNode whileStatementNode = (WhileStatementNode) node.statementNode;
            typeCheckWhileStatementNode(asts, namespace, methodNode, whileStatementNode, ctx);
        } else if (node.statementNode instanceof MethodCallNode) {
            final MethodCallNode methodCallNode = (MethodCallNode) node.statementNode;
            typeCheckMethodCallNode(asts, namespace, methodNode, methodCallNode, ctx);
        } else if (node.statementNode instanceof IfStatementNode) {
            final IfStatementNode ifStatementNode = (IfStatementNode) node.statementNode;
            typeCheckIfStatementNode(asts, namespace, methodNode, ifStatementNode, ctx);
        } else if (node.statementNode instanceof ReturnStatementNode) {
            final ReturnStatementNode returnStatementNode = (ReturnStatementNode) node.statementNode;
            typeCheckReturnStatementNode(asts, namespace, methodNode, returnStatementNode, ctx);
        } else {
            throw new Exception("unconsidered case in typechecking ");
        }
    }
}
