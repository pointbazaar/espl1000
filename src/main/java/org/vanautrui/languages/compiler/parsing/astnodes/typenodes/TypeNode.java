package org.vanautrui.languages.compiler.parsing.astnodes.typenodes;

import org.vanautrui.languages.compiler.parsing.IASTNode;

public final class TypeNode implements IASTNode {

	public ITypeNode typeNode;

	public TypeNode(){}

	public TypeNode(ITypeNode typeNode) {
		this.typeNode = typeNode;
	}

	@Override
	public String toSourceCode() {
		return this.typeNode.toSourceCode();
	}

	public String getTypeName() {
		return this.toSourceCode();
	}
}
