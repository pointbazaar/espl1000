package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.DeclaredArgumentNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST_Whole_Program;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;

import static org.vanautrui.languages.compiler.typechecking.ITypeNodeTypeChecker.typeCheckITypeNode;

public final class DeclaredArgumentNodeTypeChecker {

  //prevents instance creation
  private DeclaredArgumentNodeTypeChecker(){}

  public synchronized static void typeCheckDeclaredArgumentNode(
          final AST_Whole_Program asts,
          final NamespaceNode namespaceNode,
          final DeclaredArgumentNode declaredArgumentNode
  ) throws Exception {
    typeCheckITypeNode(asts, namespaceNode, declaredArgumentNode.type.typeNode);
  }
}
