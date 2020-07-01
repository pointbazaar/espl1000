package org.vanautrui.languages.compiler.codegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;

import java.util.ArrayList;
import java.util.List;

import static org.vanautrui.languages.compiler.codegenerator.specialized.ExpressionJavaCodeGenerator.genDracoVMCodeForExpression;

public final class ReturnJavaCodeGenerator {

    static List<String> genJavaCodeForReturn(
            final ReturnStatementNode retStmt,
            final MethodNode m,
            final SymbolTableContext ctx
    ) throws Exception {

        final List<String> vm = new ArrayList<>();
        vm.add("return " + genDracoVMCodeForExpression(retStmt.returnValue, ctx));
        return vm;
    }
}
