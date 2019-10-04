package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ArrayConstantNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.TermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.CharConstNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.FloatConstNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.IntConstNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.VariableNode;
import org.vanautrui.languages.compiler.symboltables.LocalVarSymbolTable;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;

import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.ExpressionNodeTypeChecker.typeCheckExpressionNode;
import static org.vanautrui.languages.compiler.typechecking.TypeChecker.*;
import static org.vanautrui.languages.compiler.typechecking.VariableNodeTypeChecker.typeCheckVariableNode;

public class TermNodeTypeChecker {

  static void typecheckTermNode(List<AST> asts, NamespaceNode namespaceNode, MethodNode methodNode, TermNode termNode, SubroutineSymbolTable subTable, LocalVarSymbolTable varTable) throws Exception{
    if(termNode.termNode instanceof CharConstNode) {
      //nothing to do
    }else if(termNode.termNode instanceof FloatConstNode){
      //nothing to do
    }else if(termNode.termNode instanceof IntConstNode){
      //nothing to do
    }else if(termNode.termNode instanceof ExpressionNode) {

      ExpressionNode expressionNode = (ExpressionNode) termNode.termNode;
      typeCheckExpressionNode(asts, namespaceNode,methodNode,expressionNode,subTable,varTable);

    }else if(termNode.termNode instanceof VariableNode){

      VariableNode variableNode = (VariableNode) termNode.termNode;
      typeCheckVariableNode(asts, namespaceNode,methodNode,variableNode,subTable,varTable);

    }else if(termNode.termNode instanceof MethodCallNode) {

      typeCheckMethodCallNode(asts, namespaceNode, methodNode, (MethodCallNode) termNode.termNode, subTable, varTable);
    }else if(termNode.termNode instanceof ArrayConstantNode){
      typeCheckArrayConstantNode(asts, namespaceNode,methodNode,(ArrayConstantNode) termNode.termNode,subTable,varTable);
    }else{
      throw new Exception("unhandled case "+termNode.termNode.getClass().getName());
    }
  }
}
