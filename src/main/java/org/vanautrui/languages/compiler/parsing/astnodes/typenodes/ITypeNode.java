package org.vanautrui.languages.compiler.parsing.astnodes.typenodes;

import org.vanautrui.languages.compiler.parsing.IASTNode;

public interface ITypeNode extends IASTNode {

	//such we can see if a type is primitive,
	//being passed by value instead of by reference,
	boolean isPrimitive();
}
