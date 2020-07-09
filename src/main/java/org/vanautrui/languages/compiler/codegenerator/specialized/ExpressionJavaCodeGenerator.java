package org.vanautrui.languages.compiler.codegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import static org.vanautrui.languages.compiler.codegenerator.specialized.OperatorJavaCodeGenerator.genJavaCodeForOp;
import static org.vanautrui.languages.compiler.codegenerator.specialized.TermJavaCodeGenerator.genJavaCodeForTerm;

public final class ExpressionJavaCodeGenerator {

    public static String genJavaCodeForExpression(
            final ExpressionNode expr,
            final SymbolTableContext ctx
    ) throws Exception {

        final List<String> vm=new ArrayList<>();

        vm.addAll(genJavaCodeForTerm(expr.term1, ctx));

        if(expr.term2.isPresent()){

            vm.addAll(genJavaCodeForTerm(expr.term2.get(),ctx));

            final TypeNode type = TypeResolver.getTypeTermNode(expr.term2.get(), ctx);

            final boolean hasFloatOperands = type.getTypeName().equals("Float");
            final boolean hasBoolOperands = type.getTypeName().equals("Bool");
            vm.addAll(genJavaCodeForOp(expr.op.get(),hasFloatOperands,hasBoolOperands));
        }

        return vm.stream().collect(Collectors.joining(" "));
    }
}
