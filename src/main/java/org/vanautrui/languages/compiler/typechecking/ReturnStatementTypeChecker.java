package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;
import org.vanautrui.languages.compiler.symboltables.util.SymbolTableContext;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import static org.vanautrui.languages.compiler.typechecking.ExpressionNodeTypeChecker.typeCheckExpressionNode;

public final class ReturnStatementTypeChecker {

  static void typeCheckReturnStatementNode(
          final AST_Whole_Program asts,
          final NamespaceNode namespaceNode,
          final MethodNode methodNode,
          final ReturnStatementNode returnStatementNode,
          final SymbolTableContext ctx
  ) throws Exception {

    final SubroutineSymbolTable subTable = ctx.subTable;
    final LocalVarSymbolTable varTable = ctx.varTable;
    final StructsSymbolTable structsTable = ctx.structsTable;

    //the type of the value returned should be the same as the method return type
    var returnValueType = TypeResolver.getTypeExpressionNode(returnStatementNode.returnValue, ctx);
    if (
            !(returnValueType.getTypeName().equals(methodNode.returnType.getTypeName()))

            && !contains_type(returnValueType.getTypeName(),methodNode.returnType.getTypeName())
    ) {
      throw new Exception(TypeChecker.class.getSimpleName()
              + ": return type of the method has to equal the return value type. return type '"
              + methodNode.returnType.getTypeName() + "' does not equal the returned type '" + returnValueType.getTypeName() + "'. found in method: "+methodNode.methodName);
    }
    typeCheckExpressionNode(asts, namespaceNode, methodNode, returnStatementNode.returnValue, ctx);
  }

  private static boolean contains_type(final String typename, final String maybeContainerType) {
    //Integer contains PInt, NInt
    if(maybeContainerType.equals("Integer")){
      return typename.equals("PInt") || typename.equals("NInt");
    }
    return false;
  }
}
