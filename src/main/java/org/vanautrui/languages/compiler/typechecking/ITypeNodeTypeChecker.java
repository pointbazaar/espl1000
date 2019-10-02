package org.vanautrui.languages.compiler.typechecking;

import org.apache.commons.lang3.StringUtils;
import org.vanautrui.languages.TerminalUtil;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.ClassNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ITypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.SubroutineTypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.simple.SimpleTypeNode;

import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.TypeChecker.primitive_types_and_arrays_of_them;

public class ITypeNodeTypeChecker {


  static void typeCheckITypeNode(
          List<AST> asts, ClassNode classNode,
          ITypeNode typename
  ) throws Exception {

    //if it is a simple type, check that the type is defined somewhere
    //so there should exist a class with that type

    if (typename instanceof SimpleTypeNode) {
      if (primitive_types_and_arrays_of_them.contains(typename.getTypeName())) {
        return;
      }

      for (AST ast : asts) {
        for (ClassNode myclassNode : ast.classNodeList) {
          if (myclassNode.name.getTypeName().equals(typename.getTypeName())) {
            return;
          }
        }
      }
    } else if (typename instanceof SubroutineTypeNode) {
      typeCheckITypeNode(asts, classNode, ((SubroutineTypeNode) typename).returnType);
      for (TypeNode argType : ((SubroutineTypeNode) typename).argumentTypes) {
        typeCheckITypeNode(asts, classNode, argType.typenode);
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
