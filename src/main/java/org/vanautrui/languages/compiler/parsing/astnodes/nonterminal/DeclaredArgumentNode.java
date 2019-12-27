package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;

import java.util.Optional;

public final class DeclaredArgumentNode implements IASTNode {

	public TypeNode type;

	public Optional<String> name;

	public DeclaredArgumentNode(){}

	public DeclaredArgumentNode(final TypeNode type, final Optional<String> name){
		this.type=type;
		this.name=name;
	}

	@Override
	public String toSourceCode() {
		return this.type.toSourceCode() + " " + this.name;
	}

}
