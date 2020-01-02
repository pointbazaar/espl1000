package org.vanautrui.languages.compiler.parsing.astnodes.terminal;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.ITermNode;

public final class FloatConstNode implements IASTNode, ITermNode, IConstantNode {

	//this can also have a negative value if it is preceded by a '-' operator token
	public float floatValue;

	public FloatConstNode(){}

	public FloatConstNode(final float floatValue){
		this.floatValue = floatValue;
	}

	@Override
	public String toSourceCode() {
		return this.floatValue + "";
	}

}
