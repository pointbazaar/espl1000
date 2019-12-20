package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ArrayTypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ITypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeParameterNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.BasicTypeWrappedNode;

public final class ITypeNodeTypeChecker {

  public synchronized static void typeCheckITypeNode(
          final AST_Whole_Program asts,
          final NamespaceNode namespaceNode,
          final ITypeNode typename
  ) throws Exception {

    //if it is a simple type, check that the type is defined somewhere
    //so there should exist a class with that type

    if (typename instanceof ArrayTypeNode) {
      //TODO
      throw new Exception("TODO");
    } else if (typename instanceof TypeParameterNode) {
      //TODO
      throw new Exception("TODO");
    } else if(typename instanceof BasicTypeWrappedNode){
      IBasicAndWrappedTypeNodeTypeChecker.typeCheckIBasicAndWrappedTypeNode(asts,namespaceNode, ((BasicTypeWrappedNode) typename).typenode);
      return;
    }

    final String msg = ("TYPECHECKING: ")
            + "could not find class for type: "
            + wrap(typename.toSourceCode(), "'")
            + " in file : (TODO: display file and line number)";
    throw new Exception(msg);
  }

  public static String wrap(final String s, final String wrap){
    return wrap+s+wrap;
  }
}
