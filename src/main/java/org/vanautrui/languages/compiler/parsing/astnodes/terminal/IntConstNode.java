package org.vanautrui.languages.compiler.parsing.astnodes.terminal;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.ITermNode;

public final class IntConstNode implements IASTNode, ITermNode {

	//this can also have a negative value if it is preceded by a '-' operator token
	public final int number;

	public IntConstNode(final int number) {
		this.number = number;
	}

	@Override
	public String toSourceCode() {
		return this.number + "";
	}

}
