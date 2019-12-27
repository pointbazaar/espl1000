package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes;

import org.vanautrui.languages.compiler.parsing.IASTNode;

import java.util.HashSet;
import java.util.Set;
import java.util.stream.Collectors;

public final class AST_Whole_Program implements IASTNode {

	//this contains all namespace nodes for the whole program

	public Set<NamespaceNode> namespaceNodeList = new HashSet<>();

	public AST_Whole_Program() { }

	public AST_Whole_Program(final NamespaceNode myNamespace) {
		//utility method, to facilitate creating AST_Whole_Program from a single namespace node
		this.namespaceNodeList.add(myNamespace);
	}

	@Override
	public String toSourceCode() {
		return this.namespaceNodeList
				.stream()
				.map(node -> node.toSourceCode())
				.collect(Collectors.joining("\n"));
	}
}
