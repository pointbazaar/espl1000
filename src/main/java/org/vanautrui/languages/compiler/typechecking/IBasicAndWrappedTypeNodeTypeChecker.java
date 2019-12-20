package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.StructDeclNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.IBasicAndWrappedTypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SimpleTypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SubroutineTypeNode;

import static org.vanautrui.languages.compiler.typechecking.ITypeNodeTypeChecker.wrap;
import static org.vanautrui.languages.compiler.typechecking.TypeChecker.primitive_types_and_arrays_of_them;

public final class IBasicAndWrappedTypeNodeTypeChecker {


  public synchronized static void typeCheckIBasicAndWrappedTypeNode(
          final AST_Whole_Program asts,
          final NamespaceNode namespaceNode,
          final IBasicAndWrappedTypeNode typename
  ) throws Exception {

    //if it is a simple type, check that the type is defined somewhere
    //so there should exist a class with that type

    if (typename instanceof SimpleTypeNode) {
      if (primitive_types_and_arrays_of_them.contains(typename.getTypeName())) {
        return;
      }

      for (final NamespaceNode namespace : asts.namespaceNodeList) {
        for(final StructDeclNode sdn : namespace.structDeclNodeList){
          if (sdn.getTypeName().equals(typename.getTypeName())) {
            return;
          }
        }

      }

    } else if (typename instanceof SubroutineTypeNode) {

      ITypeNodeTypeChecker.typeCheckITypeNode(asts, namespaceNode, ((SubroutineTypeNode) typename).returnType.type);
      for (final TypeNode argType : ((SubroutineTypeNode) typename).argumentTypes) {
        ITypeNodeTypeChecker.typeCheckITypeNode(asts, namespaceNode, argType.type);
      }
      return;
    }

    String msg = ("TYPECHECKING: ")
            + "could not find class for type: "
            + wrap(typename.getTypeName(), "'")
            + " in file : (TODO: display file and line number)";
    throw new Exception(msg);
  }
}
