package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.IBasicAndWrappedTypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.StatementNodeTypeChecker.typeCheckStatementNode;

public final class WhileStatementNodeTypeChecker {


  public synchronized static void typeCheckWhileStatementNode(
          List<AST> asts,
          NamespaceNode namespace,
          MethodNode methodNode,
          WhileStatementNode whileNode,
          SubroutineSymbolTable subTable,
          LocalVarSymbolTable varTable,
          StructsSymbolTable structsTable
  ) throws Exception {
    //the condition expression should be of type boolean
    var conditionType =
            TypeResolver.getTypeExpressionNode(whileNode.condition, methodNode, subTable, varTable,structsTable);

    if (!conditionType.getTypeName().equals("Bool")) {
      throw new Exception(" condition should be of type Bool : '"
              + whileNode.condition.toSourceCode()
              + "' but was of type: " + conditionType);
    }
    for (StatementNode stmt : whileNode.statements) {
      typeCheckStatementNode(asts, namespace, methodNode, stmt, subTable, varTable,structsTable);
    }
  }
}