package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.TermNode;
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

        vm.addAll(genDracoVMCodeForTerm(expr.term, ctx));

        for (int i = 0; i < expr.termNodes.size(); i++) {

            final TermNode term = expr.termNodes.get(i);

            vm.addAll(genDracoVMCodeForTerm(term,ctx));

            final TypeNode type = TypeResolver.getTypeTermNode(term, ctx);


            //TODO: this does not work well. the list of term nodes should probably be converted into a tree structure.
            //hacky
            final boolean hasFloatOperands = type.getTypeName().equals("Float");
            final boolean hasBoolOperands = type.getTypeName().equals("Bool");
            vm.addAll(genDracoVMCodeForOp(expr.operatorNodes.get(i),hasFloatOperands,hasBoolOperands));
        }

        return vm;
    }
}
