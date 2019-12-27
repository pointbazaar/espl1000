package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.StructDeclNode;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;
import org.vanautrui.languages.compiler.symboltables.structs.StructsSymbolTable;

import static org.vanautrui.languages.compiler.typechecking.MethodNodeTypeChecker.typeCheckMethodNode;
import static org.vanautrui.languages.compiler.typechecking.StructDeclNodeTypeChecker.typeCheckStructDeclNode;

public final class NamespaceNodeTypeChecker {

  //prevents instance creation
  private NamespaceNodeTypeChecker(){}

  static void typeCheckNamespaceNode(
          final AST_Whole_Program asts,
          final NamespaceNode namespace,
          final SubroutineSymbolTable subTable,
          final boolean debug,
          final StructsSymbolTable structsTable
  ) throws Exception{
    if(debug){
      System.out.println("typechecking class:"+ namespace.name);
    }
    int count=0;

    for(final NamespaceNode dragonNamespaceNode : asts.namespaceNodeList){
      if(dragonNamespaceNode.name.equals(namespace.name)){
        count++;
      }
    }

    for(final StructDeclNode structDeclNode : namespace.structs){
      typeCheckStructDeclNode(asts, namespace,structDeclNode,subTable,debug);
    }

    for(final MethodNode methodNode : namespace.methods){
      typeCheckMethodNode(asts, namespace,methodNode,subTable,structsTable);
    }

    if(count!=1){
      throw new Exception("multiple definitions of namespace '"+ namespace.name+"'");
    }
  }
}
