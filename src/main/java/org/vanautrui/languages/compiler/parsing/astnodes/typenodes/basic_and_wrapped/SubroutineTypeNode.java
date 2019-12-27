package org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped;

import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public final class SubroutineTypeNode implements IBasicAndWrappedTypeNode {

	public final TypeNode returnType;

	public final boolean has_side_effects;

	public final List<TypeNode> argumentTypes = new ArrayList<>();

	public SubroutineTypeNode(final TypeNode return_type, final boolean hasSideEffects) {
		this.returnType = return_type;
		this.has_side_effects = hasSideEffects;
	}


	@Override
	public String getTypeName() {
		return this.toSourceCode();
	}

	@Override
	public String toSourceCode() {
		return
				"("
						+ argumentTypes
						.stream()
						.map(TypeNode::getTypeName)
						.collect(Collectors.joining(","))
						+ ")"
						+ ((this.has_side_effects) ? "~>" : "->")

						+ returnType.getTypeName();

	}

	@Override
	public boolean isPrimitive() {
		return false;
	}
}
