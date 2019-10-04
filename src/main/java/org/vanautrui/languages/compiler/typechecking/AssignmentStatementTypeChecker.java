package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.AssignmentStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ITypeNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.typeresolution.TypeResolver;

import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.ExpressionNodeTypeChecker.typeCheckExpressionNode;
import static org.vanautrui.languages.compiler.typechecking.VariableNodeTypeChecker.typeCheckVariableNode;

public class AssignmentStatementTypeChecker {

  static void typeCheckAssignmentStatementNode(
          List<AST> asts, NamespaceNode namespaceNode, MethodNode methodNode,
          AssignmentStatementNode assignmentStatementNode, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable
  ) throws Exception{
    ITypeNode leftSideType = TypeResolver.getTypeVariableNode(assignmentStatementNode.variableNode,methodNode,subTable,varTable);
    ITypeNode rightSideType = TypeResolver.getTypeExpressionNode(assignmentStatementNode.expressionNode,methodNode,subTable,varTable);
    if(!leftSideType.getTypeName().equals(rightSideType.getTypeName())){
      throw new Exception(
              "with an assignment, both sides have to have the same type. here, a value of type "+rightSideType.getTypeName()+" was assigned to a value of type "+leftSideType.getTypeName()
                      +" in source: '"+assignmentStatementNode.toSourceCode()+"'"
      );
    }

    typeCheckVariableNode(asts, namespaceNode,methodNode,assignmentStatementNode.variableNode,subTable,varTable);
    typeCheckExpressionNode(asts, namespaceNode,methodNode,assignmentStatementNode.expressionNode,subTable,varTable);
  }
}
