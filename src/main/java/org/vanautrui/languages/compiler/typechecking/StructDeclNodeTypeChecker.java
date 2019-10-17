package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.StructMemberDeclNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.StructDeclNode;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;

import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.StructMemberDeclTypeChecker.typeCheckStructMemberDeclNode;

public final class StructDeclNodeTypeChecker {

  public static synchronized void typeCheckStructDeclNode (final List<AST> asts,
                                                           final NamespaceNode namespaceNode,
                                                           final StructDeclNode structDeclNode,
                                                           final SubroutineSymbolTable subroutineSymbolTable
  ) throws Exception{
    //the type of the struct is simple, by construction (see constructor)

    //check that it does not have a primitive type
    if(TypeChecker.primitive_types_and_arrays_of_them.contains(structDeclNode.structType.typeName)){
      throw new Exception("struct type cannot be primitive ");
    }

    //check that it is only declared once,
    //check that it does not have the name of a namespace
    int count=0;
    for(AST ast : asts){
      for(NamespaceNode dragonNamespaceNode : ast.namespaceNodeList){
        for(StructDeclNode structDeclNode1 : dragonNamespaceNode.structDeclNodeList){
          if(structDeclNode1.getTypeName().equals(structDeclNode.getTypeName())){
            count++;
          }
        }
        if(namespaceNode.name.getTypeName().equals(structDeclNode.getTypeName())){
          throw new Exception("struct type name must not be a namespace name");
        }
      }
    }
    if(count!=1){
      throw new Exception("struct "+structDeclNode.getTypeName()+" was declared multiple times");
    }

    //typecheck Members
    for(final StructMemberDeclNode structMemberDeclNode : structDeclNode.structMembersList){
      typeCheckStructMemberDeclNode(asts,namespaceNode,structDeclNode,structMemberDeclNode,subroutineSymbolTable);
    }
  }
}
