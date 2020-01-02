package org.vanautrui.languages.compiler.parsing.astnodes.terminal;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.ITermNode;

public final class IntConstNode implements IASTNode, ITermNode, IConstantNode {

	//this can also have a negative value if it is preceded by a '-' operator token
	public int number;

	public IntConstNode(){}

	public IntConstNode(final int number) {
		this.number = number;
	}

	@Override
	public String toSourceCode() {
		return this.number + "";
	}

}
