package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.StructDeclNode;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;

import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.MethodNodeTypeChecker.typeCheckMethodNode;
import static org.vanautrui.languages.compiler.typechecking.StructDeclNodeTypeChecker.typeCheckStructDeclNode;

public class NamespaceNodeTypeChecker {


  static void typeCheckNamespaceNode(List<AST> asts, NamespaceNode namespaceNode, SubroutineSymbolTable subroutineSymbolTable, boolean debug) throws Exception{
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

    for(StructDeclNode structDeclNode : namespaceNode.structDeclNodeList){
      typeCheckStructDeclNode(asts, namespaceNode,structDeclNode,subroutineSymbolTable);
    }

    for(MethodNode methodNode : namespaceNode.methodNodeList){
      typeCheckMethodNode(asts, namespaceNode,methodNode,subroutineSymbolTable);
    }

    if(count!=1){
      throw new Exception("multiple definitions of namespace '"+ namespaceNode.name.getTypeName()+"'");
    }
  }
}
