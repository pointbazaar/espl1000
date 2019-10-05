package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ITypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.StatementNodeTypeChecker.typeCheckStatementNode;

public final class WhileStatementNodeTypeChecker {


  public synchronized static void typeCheckWhileStatementNode(
          List<AST> asts,
          NamespaceNode namespaceNode,
          MethodNode methodNode,
          WhileStatementNode whileStatementNode,
          SubroutineSymbolTable subTable,
          LocalVarSymbolTable varTable
  ) throws Exception {
    //the condition expression should be of type boolean
    ITypeNode conditionType =
            TypeResolver.getTypeExpressionNode(whileStatementNode.condition, methodNode, subTable, varTable);

    if (!conditionType.getTypeName().equals("Bool")) {
      throw new Exception(" condition should be of type Bool : '"
              + whileStatementNode.condition.toSourceCode()
              + "' but was of type: " + conditionType);
    }
    for (StatementNode stmt : whileStatementNode.statements) {
      typeCheckStatementNode(asts, namespaceNode, methodNode, stmt, subTable, varTable);
    }
  }
}
