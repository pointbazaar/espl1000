package org.vanautrui.languages.compiler.vmcodegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;

import java.util.ArrayList;
import java.util.List;

import static org.vanautrui.languages.compiler.vmcodegenerator.specialized.ExpressionDracoVMCodeGenerator.genDracoVMCodeForExpression;

public final class ReturnDracoVMCodeGenerator {

    static List<String> genDracoVMCodeForReturn(
            ReturnStatementNode retStmt,
            MethodNode m,
            SymbolTableContext ctx
    ) throws Exception {

        final SubroutineSymbolTable subTable=ctx.subTable;
        final LocalVarSymbolTable varTable=ctx.varTable;
        final StructsSymbolTable structsTable=ctx.structsTable;

        final List<String> vm = new ArrayList<>();

        vm.addAll(genDracoVMCodeForExpression(retStmt.returnValue, ctx));
        if (m.methodName.equals("main")) {
            vm.add("exit");
        } else {

            //TODO: test the removal of local variables from the stack
            //get rid of the local variables which were pushed before
            int numberOfLocalVariablesOfSubroutine = subTable.getNumberOfLocalVariablesOfSubroutine(m.methodName);
            final String comment = "take local variables off the stack";
            for (int i = 0; i < numberOfLocalVariablesOfSubroutine; i++) {
                vm.add("swap");
                vm.add("pop");
            }

            //there is the return value on the stack,
            // we must swap that with the return address of the calling function, in order to return
            //"swap return value with return address of the calling function in order to return");
            vm.add("swap");

            vm.add("return");
        }
        return vm;
    }
}
