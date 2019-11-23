package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.TermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.IBasicAndWrappedTypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SimpleTypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.typechecking.TypeChecker;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import java.util.ArrayList;
import java.util.List;

import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.OperatorDracoVMCodeGenerator.genDracoVMCodeForOp;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.TermDracoVMCodeGenerator.genDracoVMCodeForTerm;

public final class ExpressionDracoVMCodeGenerator {

    public static List<String> genDracoVMCodeForExpression(
            ExpressionNode expr,
            SubroutineSymbolTable subTable,
            LocalVarSymbolTable varTable,
            StructsSymbolTable structsTable,
            MethodNode methodNode
    ) throws Exception {
        final List<String> vm=new ArrayList<>();

        vm.addAll(genDracoVMCodeForTerm(expr.term, subTable, varTable, structsTable));

        for (int i = 0; i < expr.termNodes.size(); i++) {

            final TermNode term = expr.termNodes.get(i);

            vm.addAll(genDracoVMCodeForTerm(term, subTable, varTable, structsTable));

            vm.addAll(genDracoVMCodeForOp(expr.operatorNodes.get(i),hasFloatOperands));
        }

        return vm;
    }
}
