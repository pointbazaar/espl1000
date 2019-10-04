package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;

import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.MethodNodeTypeChecker.typeCheckMethodNode;

public class ClassNodeTypeChecker {


  static void typeCheckClassNode(List<AST> asts, NamespaceNode namespaceNode, SubroutineSymbolTable subroutineSymbolTable, boolean debug) throws Exception{
    if(debug){
      System.out.println("typechecking class:"+ namespaceNode.name);
    }
    int count=0;
    for(AST ast : asts){
      for(NamespaceNode dragonNamespaceNode : ast.namespaceNodeList){
        if(dragonNamespaceNode.name.getTypeName().equals(namespaceNode.name.getTypeName())){
          count++;
        }
      }
    }

    for(MethodNode methodNode : namespaceNode.methodNodeList){
      typeCheckMethodNode(asts, namespaceNode,methodNode,subroutineSymbolTable);
    }

    if(count!=1){
      throw new Exception("multiple definitions of class '"+ namespaceNode.name.getTypeName()+"'");
    }
  }
}
