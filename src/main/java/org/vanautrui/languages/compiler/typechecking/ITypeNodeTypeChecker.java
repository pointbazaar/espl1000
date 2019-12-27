package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ArrayTypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ITypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeParameterNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.BasicTypeWrappedNode;

public final class ITypeNodeTypeChecker {

  //prevents instance creation
  private ITypeNodeTypeChecker(){}

  public synchronized static void typeCheckITypeNode(
          final AST_Whole_Program asts,
          final NamespaceNode namespaceNode,
          final ITypeNode typename
  ) throws Exception {

    //if it is a simple type, check that the type is defined somewhere
    //so there should exist a class with that type

    if (typename instanceof ArrayTypeNode) {

      final TypeNode element_type = ((ArrayTypeNode) typename).element_type;
      ITypeNodeTypeChecker.typeCheckITypeNode(asts,namespaceNode,element_type.typeNode);
      return;
    } else if (typename instanceof TypeParameterNode) {
      //TODO
      throw new Exception("TODO");
    } else if(typename instanceof BasicTypeWrappedNode){
      IBasicAndWrappedTypeNodeTypeChecker.typeCheckIBasicAndWrappedTypeNode(asts,namespaceNode, ((BasicTypeWrappedNode) typename).typeNode);
      return;
    }

    final String msg = ("TYPECHECKING: ")
            + "could not find struct for type: "
            + wrap(typename.toSourceCode(), "'")
            + " in file : (TODO: display file and line number)";
    throw new Exception(msg);
  }

  public static String wrap(final String s, final String wrap){
    return wrap+s+wrap;
  }
}
