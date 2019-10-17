package org.vanautrui.languages.compiler.typechecking;

import org.apache.commons.lang3.StringUtils;
import org.vanautrui.languages.TerminalUtil;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.IBasicAndWrappedTypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SubroutineTypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.BasicTypeWrappedNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SimpleTypeNode;

import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.TypeChecker.primitive_types_and_arrays_of_them;

public final class IBasicAndWrappedTypeNodeTypeChecker {


  public synchronized static void typeCheckIBasicAndWrappedTypeNode(
          List<AST> asts, NamespaceNode namespaceNode,
          IBasicAndWrappedTypeNode typename
  ) throws Exception {

    //if it is a simple type, check that the type is defined somewhere
    //so there should exist a class with that type

    if (typename instanceof SimpleTypeNode) {
      if (primitive_types_and_arrays_of_them.contains(typename.getTypeName())) {
        return;
      }

      for (AST ast : asts) {
        for (NamespaceNode myclassNode : ast.namespaceNodeList) {
          if (myclassNode.name.getTypeName().equals(typename.getTypeName())) {
            return;
          }
        }
      }
    } else if (typename instanceof SubroutineTypeNode) {

      ITypeNodeTypeChecker.typeCheckITypeNode(asts, namespaceNode, ((SubroutineTypeNode) typename).returnType.type);
      for (TypeNode argType : ((SubroutineTypeNode) typename).argumentTypes) {
        ITypeNodeTypeChecker.typeCheckITypeNode(asts, namespaceNode, argType.type);
      }
      return;
    }

    String msg = TerminalUtil.gererateErrorString("TYPECHECKING: ")
            + "could not find class for type: "
            + StringUtils.wrap(typename.getTypeName(), "'")
            + " in file : (TODO: display file and line number)";
    throw new Exception(msg);
  }
}
