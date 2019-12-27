package org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ITypeNode;

public final class BasicTypeWrappedNode implements IASTNode, ITypeNode {

	public final IBasicAndWrappedTypeNode typenode;

	public BasicTypeWrappedNode(final IBasicAndWrappedTypeNode typenode) {
		this.typenode = typenode;
	}

	public String getTypeName() {
		return this.toSourceCode();
	}

	@Override
	public String toSourceCode() {
		if (this.typenode instanceof SubroutineTypeNode) {
			return "(" + this.typenode.toSourceCode() + ")";
		} else {
			return this.typenode.toSourceCode();
		}
	}

	@Override
	public boolean isPrimitive() {
		return this.typenode.isPrimitive();
	}
}
