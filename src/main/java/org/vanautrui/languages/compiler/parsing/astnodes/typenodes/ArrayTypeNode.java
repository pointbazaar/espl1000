package org.vanautrui.languages.compiler.parsing.astnodes.typenodes;

import org.vanautrui.languages.compiler.parsing.IASTNode;

public final class ArrayTypeNode implements IASTNode, ITypeNode {

	public TypeNode element_type;

	public ArrayTypeNode(){}

	public ArrayTypeNode(final TypeNode element_type) {
		this.element_type = element_type;
	}

	@Override
	public String toSourceCode() {
		return "[" + this.element_type.toSourceCode() + "]";
	}

	@Override
	public boolean isPrimitive() {
		return false;
	}
}
