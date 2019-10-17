package org.vanautrui.languages.compiler.typechecking;

import org.apache.commons.lang3.StringUtils;
import org.vanautrui.languages.TerminalUtil;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ArrayTypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ITypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeParameterNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.BasicTypeWrappedNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.IBasicAndWrappedTypeNode;

import java.util.List;

public final class ITypeNodeTypeChecker {


  public synchronized static void typeCheckITypeNode(
          List<AST> asts, NamespaceNode namespaceNode,
          ITypeNode typename
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

    String msg = TerminalUtil.gererateErrorString("TYPECHECKING: ")
            + "could not find class for type: "
            + StringUtils.wrap(typename.toSourceCode(), "'")
            + " in file : (TODO: display file and line number)";
    throw new Exception(msg);
  }
}
