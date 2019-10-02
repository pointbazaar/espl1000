package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ITypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.ExpressionNodeTypeChecker.typeCheckExpressionNode;

public class ReturnStatementTypeChecker {

  static void typeCheckReturnStatementNode(List<AST> asts, ClassNode classNode, MethodNode methodNode, ReturnStatementNode returnStatementNode, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable) throws Exception {
    //the type of the value returned should be the same as the method return type
    ITypeNode returnValueType = TypeResolver.getTypeExpressionNode(returnStatementNode.returnValue, methodNode, subTable, varTable);
    if (
            !(returnValueType.getTypeName().equals(methodNode.returnType.getTypeName()))
    ) {
      throw new Exception(TypeChecker.class.getSimpleName()
              + ": return type of the method has to equal the return value type. return type '"
              + methodNode.returnType.getTypeName() + "' does not equal the returned type '" + returnValueType + "'");
    }
    typeCheckExpressionNode(asts, classNode, methodNode, returnStatementNode.returnValue, subTable, varTable);
  }
}
