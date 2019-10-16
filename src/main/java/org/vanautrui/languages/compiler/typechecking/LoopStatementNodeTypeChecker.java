package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.IBasicAndWrappedTypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.StatementNodeTypeChecker.typeCheckStatementNode;

public final class LoopStatementNodeTypeChecker {


  public synchronized static void typeCheckLoopStatementNode(
          List<AST> asts, NamespaceNode namespaceNode, MethodNode methodNode,
          LoopStatementNode loopStatementNode, SubroutineSymbolTable subTable,
          LocalVarSymbolTable varTable
  ) throws Exception {
    //the condition expression should be of type boolean

    var countType = TypeResolver.getTypeExpressionNode(loopStatementNode.count, methodNode, subTable, varTable);
    if (!countType.getTypeName().equals("PInt")) {
      throw
              new Exception(
                      " condition should be of an Integral Type >= 0 (PInt) . this is a loop statement after all."
              );
    }
    for (StatementNode stmt : loopStatementNode.statements) {
      typeCheckStatementNode(asts, namespaceNode, methodNode, stmt, subTable, varTable);
    }
  }
}
