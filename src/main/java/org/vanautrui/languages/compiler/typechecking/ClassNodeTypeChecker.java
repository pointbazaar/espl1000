package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;

import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.MethodNodeTypeChecker.typeCheckMethodNode;

public class ClassNodeTypeChecker {


  static void typeCheckClassNode(List<AST> asts, ClassNode classNode, SubroutineSymbolTable subroutineSymbolTable, boolean debug) throws Exception{
    if(debug){
      System.out.println("typechecking class:"+classNode.name);
    }
    int count=0;
    for(AST ast : asts){
      for(ClassNode dragonClassNode : ast.classNodeList){
        if(dragonClassNode.name.getTypeName().equals(classNode.name.getTypeName())){
          count++;
        }
      }
    }

    for(MethodNode methodNode : classNode.methodNodeList){
      typeCheckMethodNode(asts,classNode,methodNode,subroutineSymbolTable);
    }

    if(count!=1){
      throw new Exception("multiple definitions of class '"+classNode.name.getTypeName()+"'");
    }
  }
}
