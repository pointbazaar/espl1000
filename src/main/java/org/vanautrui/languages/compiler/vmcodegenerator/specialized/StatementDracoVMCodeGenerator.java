package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.IStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;

import java.util.ArrayList;
import java.util.List;

import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.AssignmentStatementDracoVMCodeGenerator.*;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.IfStatementDracoVMCodeGenerator.genVMCodeForIfStatement;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.LoopStatementDracoVMCodeGenerator.genVMCodeForLoopStatement;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.MethodCallDracoVMCodeGenerator.genVMCodeForMethodCall;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ReturnDracoVMCodeGenerator.genDracoVMCodeForReturn;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.WhileStatementDracoVMCodeGenerator.genVMCodeForWhileStatement;

public final class StatementDracoVMCodeGenerator {


    public static List<String> generateDracoVMCodeForStatement(
            StatementNode stmt,
            MethodNode containerMethod,
            SubroutineSymbolTable subTable,
            LocalVarSymbolTable varTable,
            StructsSymbolTable structsTable
    ) throws Exception {
        final List<String> vminstrs = new ArrayList<>();
        final IStatementNode snode = stmt.statementNode;

        if (snode instanceof MethodCallNode) {
            final MethodCallNode call = (MethodCallNode) snode;
            vminstrs.addAll(genVMCodeForMethodCall(call, subTable, varTable, structsTable));
            //there is no assignment, and the return value is not used in an expression,
            // so the return value should be pop'd of the stack
            vminstrs.add("pop");
        } else if (snode instanceof LoopStatementNode) {

            final LoopStatementNode loop = (LoopStatementNode) snode;

            vminstrs.addAll(genVMCodeForLoopStatement(loop, containerMethod, subTable, varTable, structsTable));

        } else if (snode instanceof AssignmentStatementNode) {

            final AssignmentStatementNode assignmentStatementNode = (AssignmentStatementNode) snode;
            vminstrs.addAll(genVMCodeForAssignmentStatement(assignmentStatementNode, subTable, varTable, structsTable));

        } else if (snode instanceof WhileStatementNode) {

            final WhileStatementNode whileStatementNode = (WhileStatementNode) snode;
            vminstrs.addAll(genVMCodeForWhileStatement(whileStatementNode, containerMethod, subTable, varTable, structsTable));

        } else if (snode instanceof IfStatementNode) {

            final IfStatementNode ifStatementNode = (IfStatementNode) snode;
            vminstrs.addAll(genVMCodeForIfStatement(ifStatementNode, containerMethod, subTable, varTable, structsTable));

        } else if (snode instanceof ReturnStatementNode) {

            final ReturnStatementNode returnStatementNode = (ReturnStatementNode) snode;
            vminstrs.addAll(genDracoVMCodeForReturn(returnStatementNode, containerMethod, subTable, varTable, structsTable));

        } else {
            throw new Exception("unconsidered statement type: " + stmt.statementNode.getClass().getName());
        }

        return vminstrs;
    }
}
