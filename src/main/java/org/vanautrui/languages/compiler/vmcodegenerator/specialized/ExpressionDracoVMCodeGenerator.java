package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import java.util.ArrayList;
import java.util.List;

import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.OperatorDracoVMCodeGenerator.genDracoVMCodeForOp;
import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.TermDracoVMCodeGenerator.genDracoVMCodeForTerm;

public final class ExpressionDracoVMCodeGenerator {

    public static List<String> genDracoVMCodeForExpression(
            final ExpressionNode expr,
            final SymbolTableContext ctx
    ) throws Exception {

        final SubroutineSymbolTable subTable=ctx.subTable;
        final LocalVarSymbolTable varTable=ctx.varTable;
        final StructsSymbolTable structsTable=ctx.structsTable;

        final List<String> vm=new ArrayList<>();

        vm.addAll(genDracoVMCodeForTerm(expr.term1, ctx));

        if(expr.term2.isPresent()){

            vm.addAll(genDracoVMCodeForTerm(expr.term2.get(),ctx));

            final TypeNode type = TypeResolver.getTypeTermNode(expr.term2.get(), ctx);

            final boolean hasFloatOperands = type.getTypeName().equals("Float");
            final boolean hasBoolOperands = type.getTypeName().equals("Bool");
            vm.addAll(genDracoVMCodeForOp(expr.op.get(),hasFloatOperands,hasBoolOperands));
        }

        return vm;
    }
}
