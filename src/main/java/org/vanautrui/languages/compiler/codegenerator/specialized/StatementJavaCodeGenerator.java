package org.vanautrui.languages.compiler.codegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.IStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;

import java.util.ArrayList;
import java.util.List;

import static org.vanautrui.languages.compiler.codegenerator.specialized.AssignmentStatementJavaCodeGenerator.genVMCodeForAssignmentStatement;
import static org.vanautrui.languages.compiler.codegenerator.specialized.IfStatementJavaCodeGenerator.genVMCodeForIfStatement;
import static org.vanautrui.languages.compiler.codegenerator.specialized.LoopStatementJavaCodeGenerator.genJavaCodeForLoopStatement;
import static org.vanautrui.languages.compiler.codegenerator.specialized.MethodCallJavaCodeGenerator.genVMCodeForMethodCall;
import static org.vanautrui.languages.compiler.codegenerator.specialized.ReturnJavaCodeGenerator.genJavaCodeForReturn;
import static org.vanautrui.languages.compiler.codegenerator.specialized.WhileStatementJavaCodeGenerator.genJavaCodeForWhileStatement;

final class StatementJavaCodeGenerator {

    static List<String> generateJavaCodeForStatement(
            final StatementNode stmt,
            final MethodNode m,
            final SymbolTableContext ctx
    ) throws Exception {

        final List<String> vminstrs = new ArrayList<>();
        final IStatementNode snode = stmt.statementNode;

        //TODO: manage the adding of ';' at the end of a statement in this method exclusively

        if (snode instanceof MethodCallNode call) {
            vminstrs.add(genVMCodeForMethodCall(call, ctx)+";");
        } else if (snode instanceof LoopStatementNode loop) {
            vminstrs.addAll(genJavaCodeForLoopStatement(loop, m, ctx));
        } else if (snode instanceof AssignmentStatementNode assignmentStatementNode) {
            vminstrs.addAll(genVMCodeForAssignmentStatement(assignmentStatementNode, ctx));
        } else if (snode instanceof WhileStatementNode whileStatementNode) {
            vminstrs.addAll(genJavaCodeForWhileStatement(whileStatementNode, m, ctx));
        } else if (snode instanceof IfStatementNode ifStatementNode) {
            vminstrs.addAll(genVMCodeForIfStatement(ifStatementNode, m, ctx));
        } else if (snode instanceof ReturnStatementNode returnStatementNode) {
            vminstrs.addAll(genJavaCodeForReturn(returnStatementNode, ctx));
        } else {
            throw new Exception("unconsidered statement type: " + stmt.statementNode.getClass().getName());
        }

        return vminstrs;
    }
}
