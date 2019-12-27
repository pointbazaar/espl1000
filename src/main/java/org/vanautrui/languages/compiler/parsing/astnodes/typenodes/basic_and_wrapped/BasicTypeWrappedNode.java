package org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ITypeNode;

public final class BasicTypeWrappedNode implements IASTNode, ITypeNode {

	public IBasicAndWrappedTypeNode typeNode;

	public BasicTypeWrappedNode(){}

	public BasicTypeWrappedNode(final IBasicAndWrappedTypeNode typeNode) {
		this.typeNode = typeNode;
	}

	public String getTypeName() {
		return this.toSourceCode();
	}

	@Override
	public String toSourceCode() {
		if (this.typeNode instanceof SubroutineTypeNode) {
			return "(" + this.typeNode.toSourceCode() + ")";
		} else {
			return this.typeNode.toSourceCode();
		}
	}

	@Override
	public boolean isPrimitive() {
		return this.typeNode.isPrimitive();
	}
}
