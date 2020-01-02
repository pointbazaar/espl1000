package org.vanautrui.languages.compiler.parsing.astnodes.terminal;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.ITermNode;

public final class CharConstNode implements IASTNode, ITermNode, IConstantNode {

	public char content;

	public CharConstNode(){}

	public CharConstNode(final char content){
		this.content=content;
	}

	@Override
	public String toSourceCode() {
		return this.content + "";
	}
}
