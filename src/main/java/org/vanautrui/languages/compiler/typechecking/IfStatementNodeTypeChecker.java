package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ITypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.StatementNodeTypeChecker.typeCheckStatementNode;

public final class IfStatementNodeTypeChecker {


  public static void typeCheckIfStatementNode(List<AST> asts, NamespaceNode namespaceNode,
                                       MethodNode methodNode,
                                       IfStatementNode ifStatementNode,
                                       SubroutineSymbolTable subTable,
                                       LocalVarSymbolTable varTable) throws Exception {
    //the condition expression should be of type boolean
    ITypeNode conditionType =
            TypeResolver.getTypeExpressionNode(ifStatementNode.condition, methodNode, subTable, varTable);

    if (!conditionType.getTypeName().equals("Bool")) {
      throw new Exception(" condition should be of type Bool, but is of type: " + conditionType.getTypeName());
    }
    for (StatementNode stmt : ifStatementNode.statements) {
      typeCheckStatementNode(asts, namespaceNode, methodNode, stmt, subTable, varTable);
    }
    for (StatementNode stmt : ifStatementNode.elseStatements) {
      typeCheckStatementNode(asts, namespaceNode, methodNode, stmt, subTable, varTable);
    }
  }
}
