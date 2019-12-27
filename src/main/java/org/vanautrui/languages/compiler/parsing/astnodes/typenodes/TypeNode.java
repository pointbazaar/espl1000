package org.vanautrui.languages.compiler.parsing.astnodes.typenodes;

import org.vanautrui.languages.compiler.parsing.IASTNode;

public final class TypeNode implements IASTNode {

	public ITypeNode type;

	public TypeNode(ITypeNode type) {
		this.type = type;
	}


	@Override
	public String toSourceCode() {
		return this.type.toSourceCode();
	}

	public String getTypeName() {
		return this.toSourceCode();
	}
}
