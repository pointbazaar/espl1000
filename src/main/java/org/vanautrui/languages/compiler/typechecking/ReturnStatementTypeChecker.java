package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.IBasicAndWrappedTypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.ExpressionNodeTypeChecker.typeCheckExpressionNode;

public class ReturnStatementTypeChecker {

  static void typeCheckReturnStatementNode(
          List<AST> asts,
          NamespaceNode namespaceNode,
          MethodNode methodNode,
          ReturnStatementNode returnStatementNode,
          SubroutineSymbolTable subTable,
          LocalVarSymbolTable varTable,
          StructsSymbolTable structsTable
  ) throws Exception {
    //the type of the value returned should be the same as the method return type
    var returnValueType = TypeResolver.getTypeExpressionNode(returnStatementNode.returnValue, methodNode, subTable, varTable,structsTable);
    if (
            !(returnValueType.getTypeName().equals(methodNode.returnType.getTypeName()))
    ) {
      throw new Exception(TypeChecker.class.getSimpleName()
              + ": return type of the method has to equal the return value type. return type '"
              + methodNode.returnType.getTypeName() + "' does not equal the returned type '" + returnValueType + "'");
    }
    typeCheckExpressionNode(asts, namespaceNode, methodNode, returnStatementNode.returnValue, subTable, varTable,structsTable);
  }
}
