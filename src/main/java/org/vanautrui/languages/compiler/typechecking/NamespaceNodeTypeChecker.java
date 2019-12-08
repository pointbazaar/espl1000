package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.StructDeclNode;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;

import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.MethodNodeTypeChecker.typeCheckMethodNode;
import static org.vanautrui.languages.compiler.typechecking.StructDeclNodeTypeChecker.typeCheckStructDeclNode;

public final class NamespaceNodeTypeChecker {


  static void typeCheckNamespaceNode(
          AST_Whole_Program asts,
          NamespaceNode namespace,
          SubroutineSymbolTable subTable,
          boolean debug,
          StructsSymbolTable structsTable
  ) throws Exception{
    if(debug){
      System.out.println("typechecking class:"+ namespace.name);
    }
    int count=0;

    for(NamespaceNode dragonNamespaceNode : asts.namespaceNodeList){
      if(dragonNamespaceNode.name.equals(namespace.name)){
        count++;
      }
    }

    for(StructDeclNode structDeclNode : namespace.structDeclNodeList){
      typeCheckStructDeclNode(asts, namespace,structDeclNode,subTable,debug);
    }

    for(MethodNode methodNode : namespace.methodNodeList){
      typeCheckMethodNode(asts, namespace,methodNode,subTable,structsTable);
    }

    if(count!=1){
      throw new Exception("multiple definitions of namespace '"+ namespace.name+"'");
    }
  }
}
