package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.BasicTypeWrappedNode;

public final class StructMemberDeclNode implements IASTNode {

	public BasicTypeWrappedNode type;
	public String name;

	public StructMemberDeclNode(){}

	public StructMemberDeclNode(final BasicTypeWrappedNode type, final String name){
		this.type=type;
		this.name=name;
	}

	@Override
	public String toSourceCode() {
		return this.type.toSourceCode() + " " + this.name;
	}
}
