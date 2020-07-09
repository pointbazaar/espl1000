package org.vanautrui.languages.compiler.simplify;

import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.MethodNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.NamespaceNode;

import java.util.stream.Collectors;

public final class Simplifier {

	public static NamespaceNode simplifyNamespaceNode(
			final NamespaceNode namespaceNode
	){
		return namespaceNode;
	}

}
