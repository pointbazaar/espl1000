package org.vanautrui.languages.compiler.codegenerator.specialized;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import static org.vanautrui.languages.compiler.codegenerator.specialized.ExpressionJavaCodeGenerator.genDracoVMCodeForExpression;

public final class MethodCallJavaCodeGenerator {

    public static String genVMCodeForMethodCall(
            final MethodCallNode methodCallNode,
            final SymbolTableContext ctx
    ) throws Exception {

        final SubroutineSymbolTable subTable=ctx.subTable;
        final LocalVarSymbolTable varTable=ctx.varTable;

        if (varTable.containsVariable(methodCallNode.methodName)) {
            //compile call to subroutine which is given as local variable or argument
            //TODO: currently i am unsure how to implement this in java code
            throw new RuntimeException("NOT IMPLEMENTED");
        } else if (subTable.containsSubroutine(methodCallNode.methodName)) {

            final String methodName = subTable.getContainingClassName(methodCallNode.methodName)+"."+methodCallNode.methodName;

            final String s =
              methodCallNode.arguments.stream().map(expr -> {
                  try {
                      return genDracoVMCodeForExpression(expr,ctx);
                  } catch (Exception e) {
                      e.printStackTrace();
                      throw new RuntimeException(e);
                  }
              }).collect(Collectors.joining(","))
            ;
            return methodName+"("+s+")";

        } else {
            throw new Exception("subroutine " + methodCallNode.methodName + " not found in local variables and also not found in subroutines");
        }
    }
}
