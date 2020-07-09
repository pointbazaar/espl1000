package org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ITypeNode;

public interface IBasicAndWrappedTypeNode extends IASTNode, ITypeNode {

	//classes implementing this interface are also used to communicate type information throughout the compiler.
	//so it is not just for parsing.

	String getTypeName();
}
