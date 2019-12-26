package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import static org.vanautrui.languages.compiler.typechecking.StatementNodeTypeChecker.typeCheckStatementNode;

public final class LoopStatementNodeTypeChecker {


  public synchronized static void typeCheckLoopStatementNode(
          final AST_Whole_Program asts,
          final NamespaceNode namespaceNode,
          final MethodNode methodNode,
          final LoopStatementNode loopStatementNode,
          final SymbolTableContext ctx
    ) throws Exception {
    final SubroutineSymbolTable subTable = ctx.subTable;
    final LocalVarSymbolTable varTable = ctx.varTable;
    final StructsSymbolTable structsTable = ctx.structsTable;

    //the condition expression should be of type boolean

    final var countType = TypeResolver.getTypeExpressionNode(loopStatementNode.count, ctx);
    if (!countType.getTypeName().equals("PInt")) {
      throw
              new Exception(
                      " condition should be of an Integral Type >= 0 (PInt) . this is a loop statement after all."
              );
    }
    for (final StatementNode stmt : loopStatementNode.statements) {
      typeCheckStatementNode(asts, namespaceNode, methodNode, stmt, ctx);
    }
  }
}
