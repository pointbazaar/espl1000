package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import static org.vanautrui.languages.compiler.typechecking.StatementNodeTypeChecker.typeCheckStatementNode;

public final class IfStatementNodeTypeChecker {

  public static void typeCheckIfStatementNode(
          final AST_Whole_Program asts,
          final NamespaceNode namespaceNode,
          final MethodNode methodNode,
          final IfStatementNode ifStatementNode,
          final SubroutineSymbolTable subTable,
          final LocalVarSymbolTable varTable,
          final StructsSymbolTable structsTable
  ) throws Exception {
    //the condition expression should be of type boolean
    final var conditionType =
      TypeResolver.getTypeExpressionNode(ifStatementNode.condition, subTable, varTable,structsTable);

    if (!conditionType.getTypeName().equals("Bool")) {
      throw new Exception(" condition should be of type Bool, but is of type: " + conditionType.getTypeName()+" in : "+ifStatementNode.condition.toSourceCode());
    }
    for (StatementNode stmt : ifStatementNode.statements) {
      typeCheckStatementNode(asts, namespaceNode, methodNode, stmt, subTable, varTable,structsTable);
    }
    for (StatementNode stmt : ifStatementNode.elseStatements) {
      typeCheckStatementNode(asts, namespaceNode, methodNode, stmt, subTable, varTable,structsTable);
    }
  }
}
