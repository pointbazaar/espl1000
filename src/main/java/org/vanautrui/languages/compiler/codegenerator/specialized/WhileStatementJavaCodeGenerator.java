package org.vanautrui.languages.compiler.codegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import static org.vanautrui.languages.compiler.codegenerator.specialized.ExpressionJavaCodeGenerator.genJavaCodeForExpression;
import static org.vanautrui.languages.compiler.codegenerator.specialized.StatementJavaCodeGenerator.generateJavaCodeForStatement;

public final class WhileStatementJavaCodeGenerator {


    public static List<String> genJavaCodeForWhileStatement(
            final WhileStatementNode whileStmt,
            final MethodNode containerMethod,
            final SymbolTableContext ctx
    ) throws Exception {

        final List<String> vm = new ArrayList<>();
        vm.add("while ("+ genJavaCodeForExpression(whileStmt.condition, ctx)+") {");

        for (StatementNode stmt : whileStmt.statements) {
            vm.addAll(
                generateJavaCodeForStatement(stmt, containerMethod, ctx)
                    .stream()
                    .map(s -> "\t"+s) //indent correctly
                    .collect(Collectors.toList())
            );
        }

        vm.add("}");

        return vm;
    }
}
