package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import static org.vanautrui.languages.compiler.typechecking.StatementNodeTypeChecker.typeCheckStatementNode;

public final class IfStatementNodeTypeChecker {

  //prevents instance creation
  private IfStatementNodeTypeChecker(){}

  public static void typeCheckIfStatementNode(
          final AST_Whole_Program asts,
          final NamespaceNode namespaceNode,
          final MethodNode methodNode,
          final IfStatementNode ifStatementNode,
          final SymbolTableContext ctx
  ) throws Exception {


    //the condition expression should be of type boolean
    final var conditionType =
      TypeResolver.getTypeExpressionNode(ifStatementNode.condition, ctx);

    if (!conditionType.getTypeName().equals("Bool")) {
      throw new Exception(" condition should be of type Bool, but is of type: " + conditionType.getTypeName()+" in : "+ifStatementNode.condition.toSourceCode());
    }
    for (StatementNode stmt : ifStatementNode.statements) {
      typeCheckStatementNode(asts, namespaceNode, methodNode, stmt, ctx);
    }
    for (StatementNode stmt : ifStatementNode.elseStatements) {
      typeCheckStatementNode(asts, namespaceNode, methodNode, stmt, ctx);
    }
  }
}
