package org.vanautrui.languages.compiler.parsing.astnodes.terminal;

import org.vanautrui.languages.compiler.parsing.IASTNode;

public final class IdentifierNode implements IASTNode {

	//this identifies a variable
	public final String name;

	public IdentifierNode(final String name){
		this.name=name;
	}

	@Override
	public String toSourceCode() {
		return this.name;
	}

}
