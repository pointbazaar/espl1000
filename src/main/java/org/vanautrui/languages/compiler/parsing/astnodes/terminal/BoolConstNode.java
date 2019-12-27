package org.vanautrui.languages.compiler.parsing.astnodes.terminal;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.ITermNode;

public final class BoolConstNode implements IASTNode, ITermNode {

	public final boolean value;

	public BoolConstNode(final boolean value){
		this.value=value;
	}

	@Override
	public String toSourceCode() {
		return this.value + "";
	}

}
