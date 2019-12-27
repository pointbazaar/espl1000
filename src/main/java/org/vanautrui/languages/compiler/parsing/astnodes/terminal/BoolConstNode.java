package org.vanautrui.languages.compiler.parsing.astnodes.terminal;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.ITermNode;

public final class BoolConstNode implements IASTNode, ITermNode {

	public final boolean boolValue;

	public BoolConstNode(final boolean boolValue){
		this.boolValue = boolValue;
	}

	@Override
	public String toSourceCode() {
		return this.boolValue + "";
	}

}
