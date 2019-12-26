package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import static org.vanautrui.languages.compiler.typechecking.StatementNodeTypeChecker.typeCheckStatementNode;

public final class WhileStatementNodeTypeChecker {


  public synchronized static void typeCheckWhileStatementNode(
          final AST_Whole_Program asts,
          final NamespaceNode namespace,
          final MethodNode methodNode,
          final WhileStatementNode whileNode,
          final SymbolTableContext ctx
  ) throws Exception {

    //the condition expression should be of type boolean
    var conditionType =
            TypeResolver.getTypeExpressionNode(whileNode.condition, ctx);

    if (!conditionType.getTypeName().equals("Bool")) {
      throw new Exception(" condition should be of type Bool : '"
              + whileNode.condition.toSourceCode()
              + "' but was of type: " + conditionType);
    }
    for (final StatementNode stmt : whileNode.statements) {
      typeCheckStatementNode(asts, namespace, methodNode, stmt, ctx);
    }
  }
}
