package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;

import java.util.ArrayList;
import java.util.List;

import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.OperatorDracoVMCodeGenerator.genDracoVMCodeForOp;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.TermDracoVMCodeGenerator.genDracoVMCodeForTerm;

public final class ExpressionDracoVMCodeGenerator {

    public static List<String> genDracoVMCodeForExpression(
            ExpressionNode expr,
            SubroutineSymbolTable subTable, LocalVarSymbolTable varTable,
            StructsSymbolTable structsTable
    ) throws Exception {
        final List<String> vm=new ArrayList<>();

        vm.addAll(genDracoVMCodeForTerm(expr.term, subTable, varTable, structsTable));

        for (int i = 0; i < expr.termNodes.size(); i++) {

            vm.addAll(genDracoVMCodeForTerm(expr.termNodes.get(i), subTable, varTable, structsTable));
            vm.addAll(genDracoVMCodeForOp(expr.operatorNodes.get(i)));
        }

        return vm;
    }
}
