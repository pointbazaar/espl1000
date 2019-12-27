package org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped;

import java.util.ArrayList;
import java.util.Arrays;

public final class SimpleTypeNode implements IBasicAndWrappedTypeNode {

	public String typeName;

	public SimpleTypeNode(){}

	public SimpleTypeNode(String typeName) {
		this.typeName = typeName;
	}

	public final boolean isAnyType() {
		//if this is a type wildcard '#'
		return this.typeName.equals("#");
	}

	@Override
	public String toSourceCode() {
		return this.getTypeName();
	}

	@Override
	public String getTypeName() {
		return this.typeName;
	}

	@Override
	public boolean isPrimitive() {

		final String[] primitive_types = new String[]{"PInt", "Int", "NInt", "Float", "Bool", "Char"};

		return  (new ArrayList<>(Arrays.asList(primitive_types))).contains(this.typeName);
	}
}
