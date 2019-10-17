package org.vanautrui.languages.compiler.typechecking;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.DeclaredArgumentNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.AST;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;

import java.util.List;

import static org.vanautrui.languages.compiler.typechecking.ITypeNodeTypeChecker.typeCheckITypeNode;

public final class DeclaredArgumentNodeTypeChecker {

  public synchronized static void typeCheckDeclaredArgumentNode(List<AST> asts,
                                            NamespaceNode namespaceNode,
                                            DeclaredArgumentNode declaredArgumentNode) throws Exception {
    typeCheckITypeNode(asts, namespaceNode, declaredArgumentNode.type.type);
  }
}
