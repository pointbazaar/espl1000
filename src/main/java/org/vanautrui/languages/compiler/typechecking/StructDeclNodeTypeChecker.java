package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.StructMemberDeclNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.StructDeclNode;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import static org.vanautrui.languages.compiler.typechecking.StructMemberDeclTypeChecker.typeCheckStructMemberDeclNode;

public final class StructDeclNodeTypeChecker {

  public static synchronized void typeCheckStructDeclNode (
          final List<AST> asts,
          final NamespaceNode namespaceNode,
          final StructDeclNode structDeclNode,
          final SubroutineSymbolTable subroutineSymbolTable,
          final boolean debug
  ) throws Exception{
    //the type of the struct is simple, by construction (see constructor)

    //check that it does not have a primitive type
    if(TypeChecker.primitive_types_and_arrays_of_them.contains(structDeclNode.structType.getTypeName())){
      throw new Exception("struct type cannot be primitive ");
    }

    final List<NamespaceNode> namespaces_where_struct_was_declared = new ArrayList<>();

    //check that it is only declared once,
    //check that it does not have the name of a namespace
    int count=0;
    for(AST ast : asts){
      for(NamespaceNode namespace : ast.namespaceNodeList){
        for(StructDeclNode structDeclNode1 : namespace.structDeclNodeList){
          if(structDeclNode1.getTypeName().equals(structDeclNode.getTypeName())){
            if(debug){
              System.out.println(structDeclNode.getTypeName()+" declared in namespace "+namespace.name.getTypeName());
            }
            namespaces_where_struct_was_declared.add(namespace);
            count++;
          }
        }
        if(namespaceNode.name.getTypeName().equals(structDeclNode.getTypeName())){
          throw new Exception("struct type name must not be a namespace name");
        }
      }
    }
    if(count>1){
      throw
              new Exception(
                      "struct "
                              +structDeclNode.getTypeName()
                              +" was declared multiple times, in namespaces: "
                              +namespaces_where_struct_was_declared
                              .stream()
                              .map(ns->ns.name.getTypeName())
                              .collect(Collectors.joining(","))
              );
    }
    if(count==0){
      throw new Exception("struct "+structDeclNode.getTypeName()+" was not declared.");
    }
    //typecheck Members
    for(final StructMemberDeclNode structMemberDeclNode : structDeclNode.structMembersList){
      typeCheckStructMemberDeclNode(asts,namespaceNode,structDeclNode,structMemberDeclNode,subroutineSymbolTable);
    }
  }
}
