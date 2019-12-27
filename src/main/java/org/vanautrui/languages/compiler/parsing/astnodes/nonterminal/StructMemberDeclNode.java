package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.BasicTypeWrappedNode;

public final class StructMemberDeclNode implements IASTNode {

	public BasicTypeWrappedNode typeNode;
	public String name;

	public StructMemberDeclNode(){}

	public StructMemberDeclNode(final BasicTypeWrappedNode typeNode, final String name){
		this.typeNode = typeNode;
		this.name=name;
	}

	@Override
	public String toSourceCode() {
		return this.typeNode.toSourceCode() + " " + this.name;
	}
}
