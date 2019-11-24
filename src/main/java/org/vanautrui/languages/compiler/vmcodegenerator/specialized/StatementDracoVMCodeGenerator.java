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
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;

import java.util.ArrayList;
import java.util.List;

import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.AssignmentStatementDracoVMCodeGenerator.*;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.IfStatementDracoVMCodeGenerator.genVMCodeForIfStatement;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.LoopStatementDracoVMCodeGenerator.genVMCodeForLoopStatement;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.MethodCallDracoVMCodeGenerator.genVMCodeForMethodCall;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ReturnDracoVMCodeGenerator.genDracoVMCodeForReturn;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.WhileStatementDracoVMCodeGenerator.genVMCodeForWhileStatement;

final class StatementDracoVMCodeGenerator {


    static List<String> generateDracoVMCodeForStatement(
            StatementNode stmt,
            MethodNode m,
            SymbolTableContext ctx
    ) throws Exception {

        final SubroutineSymbolTable subTable=ctx.subTable;
        final LocalVarSymbolTable varTable=ctx.varTable;
        final StructsSymbolTable structsTable=ctx.structsTable;

        final List<String> vminstrs = new ArrayList<>();
        final IStatementNode snode = stmt.statementNode;

        if (snode instanceof MethodCallNode) {
            final MethodCallNode call = (MethodCallNode) snode;
            vminstrs.addAll(genVMCodeForMethodCall(call, ctx));
            //there is no assignment, and the return value is not used in an expression,
            // so the return value should be pop'd of the stack
            vminstrs.add("pop");
        } else if (snode instanceof LoopStatementNode) {

            final LoopStatementNode loop = (LoopStatementNode) snode;

            vminstrs.addAll(genVMCodeForLoopStatement(loop, m, ctx));

        } else if (snode instanceof AssignmentStatementNode) {

            final AssignmentStatementNode assignmentStatementNode = (AssignmentStatementNode) snode;
            vminstrs.addAll(genVMCodeForAssignmentStatement(assignmentStatementNode, ctx));

        } else if (snode instanceof WhileStatementNode) {

            final WhileStatementNode whileStatementNode = (WhileStatementNode) snode;
            vminstrs.addAll(genVMCodeForWhileStatement(whileStatementNode, m, ctx));

        } else if (snode instanceof IfStatementNode) {

            final IfStatementNode ifStatementNode = (IfStatementNode) snode;
            vminstrs.addAll(genVMCodeForIfStatement(ifStatementNode, m, ctx));

        } else if (snode instanceof ReturnStatementNode) {

            final ReturnStatementNode returnStatementNode = (ReturnStatementNode) snode;
            vminstrs.addAll(genDracoVMCodeForReturn(returnStatementNode, m, ctx));

        } else {
            throw new Exception("unconsidered statement type: " + stmt.statementNode.getClass().getName());
        }

        return vminstrs;
    }
}
