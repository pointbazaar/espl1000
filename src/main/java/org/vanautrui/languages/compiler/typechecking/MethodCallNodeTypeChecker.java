package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SubroutineTypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;

import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.ExpressionNodeTypeChecker.typeCheckExpressionNode;

public final class MethodCallNodeTypeChecker {


  public synchronized static void typeCheckMethodCallNode(List<AST> asts, NamespaceNode namespaceNode,
                                      MethodNode methodNode, MethodCallNode methodCallNode,
                                      SubroutineSymbolTable subTable, LocalVarSymbolTable varTable) throws Exception {
    boolean found = false;

    if (subTable.containsSubroutine(methodCallNode.methodName)) {
      found = true;
    }

    if (
            varTable.containsVariable(methodCallNode.methodName)
                    && varTable.get(methodCallNode.methodName).getType().type instanceof SubroutineTypeNode
    ) {
      found = true;
    }

    if (!found) {
      System.out.println(subTable.toString());
      throw
              new Exception(
                      "name of method not in subroutine symbol table and not in local variable table (or not a subroutine variable): "
                              + methodCallNode.methodName
              );
    }

    //for static method calls, check that the class exists

    //all arguments should typecheck
    for (ExpressionNode expr : methodCallNode.argumentList) {
      typeCheckExpressionNode(asts, namespaceNode, methodNode, expr, subTable, varTable);
    }
  }
}
