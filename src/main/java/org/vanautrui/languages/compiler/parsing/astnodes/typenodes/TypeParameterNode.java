package org.vanautrui.languages.compiler.parsing.astnodes.typenodes;

import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.IBasicAndWrappedTypeNode;

public final class TypeParameterNode implements IBasicAndWrappedTypeNode, ITypeNode {

	public int typeParameterIndex;

	public TypeParameterNode(){}

	public TypeParameterNode(final int typeParameterIndex){
		this.typeParameterIndex=typeParameterIndex;
	}

	@Override
	public String toSourceCode() {
		return "?T" + this.typeParameterIndex;
	}

	@Override
	public String getTypeName() {
		return this.toSourceCode();
	}

	@Override
	public boolean isPrimitive() {
		//TODO: fix this. we do not know. we return false, but the real type could be primitive.
		return false;
	}
}
