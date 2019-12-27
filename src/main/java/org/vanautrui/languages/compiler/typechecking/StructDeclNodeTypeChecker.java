package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.StructMemberDeclNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.StructDeclNode;
import org.vanautrui.languages.compiler.symboltables.SubroutineSymbolTable;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

import static org.vanautrui.languages.compiler.typechecking.StructMemberDeclTypeChecker.typeCheckStructMemberDeclNode;

public final class StructDeclNodeTypeChecker {

  //prevents instance creation
  private StructDeclNodeTypeChecker(){}

  public static synchronized void typeCheckStructDeclNode (
          final AST_Whole_Program asts,
          final NamespaceNode namespaceNode,
          final StructDeclNode structDeclNode,
          final SubroutineSymbolTable subroutineSymbolTable,
          final boolean debug
  ) throws Exception{
    //the type of the struct is simple, by construction (see constructor)

    //check that it does not have a primitive type
    if(TypeChecker.primitive_types_and_arrays_of_them.contains(structDeclNode.type.getTypeName())){
      throw new Exception("struct type cannot be primitive ");
    }

    final List<NamespaceNode> namespaces_where_struct_was_declared = new ArrayList<>();

    //check that it is only declared once,
    //check that it does not have the name of a namespace
    int count=0;
    for(NamespaceNode namespace : asts.namespaceNodeList){
      for(StructDeclNode structDeclNode1 : namespace.structs){
        if(structDeclNode1.type.getTypeName().equals(structDeclNode.type.getTypeName())){
          if(debug){
            System.out.println(structDeclNode.type.getTypeName()+" declared in namespace "+namespace.name);
          }
          namespaces_where_struct_was_declared.add(namespace);
          count++;
        }
      }
      if(namespaceNode.name.equals(structDeclNode.type.getTypeName())){
        throw new Exception("struct type name must not be a namespace name");
      }
    }

    if(count>1){
      throw
              new Exception(
                      "struct "
                              +structDeclNode.type.getTypeName()
                              +" was declared multiple times, in namespaces: "
                              +namespaces_where_struct_was_declared
                              .stream()
                              .map(ns->ns.name)
                              .collect(Collectors.joining(","))
              );
    }
    if(count==0){
      throw new Exception("struct "+structDeclNode.type.getTypeName()+" was not declared.");
    }
    //typecheck Members
    for(final StructMemberDeclNode structMemberDeclNode : structDeclNode.members){
      typeCheckStructMemberDeclNode(asts,namespaceNode,structDeclNode,structMemberDeclNode,subroutineSymbolTable);
    }
  }
}
