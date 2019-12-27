package org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Optional;

public final class SimpleTypeNode implements IBasicAndWrappedTypeNode {

	private final Optional<String> typeName;

	public SimpleTypeNode(String typeName) {
		this.typeName = Optional.of(typeName);
	}

	public final boolean isAnyType() {
		//if this is a type wildcard '#'
		return !this.typeName.isPresent();
	}

	@Override
	public String toSourceCode() {
		return this.getTypeName();
	}

	@Override
	public String getTypeName() {
		return this.typeName.orElse("#");
	}

	@Override
	public boolean isPrimitive() {

		final String[] primitive_types = new String[]{"PInt", "Int", "NInt", "Float", "Bool", "Char"};

		return this.typeName.isPresent() && (new ArrayList<>(Arrays.asList(primitive_types))).contains(this.typeName.get());
	}
}
