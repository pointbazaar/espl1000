package org.vanautrui.languages.compiler.parsing.astnodes.terminal;

import org.vanautrui.languages.compiler.parsing.IASTNode;

public final class IdentifierNode implements IASTNode {

	//this identifies a variable
	public final String identifier;

	public IdentifierNode(final String identifier){
		this.identifier = identifier;
	}

	@Override
	public String toSourceCode() {
		return this.identifier;
	}

}
