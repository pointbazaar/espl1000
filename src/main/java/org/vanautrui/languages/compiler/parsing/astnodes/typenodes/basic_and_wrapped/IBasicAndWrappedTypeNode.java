package org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped;

import com.fasterxml.jackson.annotation.JsonSubTypes;
import com.fasterxml.jackson.annotation.JsonTypeInfo;
import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ITypeNode;

@JsonTypeInfo(
		use = JsonTypeInfo.Id.NAME,
		include = JsonTypeInfo.As.PROPERTY,
		property = "type")
@JsonSubTypes({
		@JsonSubTypes.Type(value = SimpleTypeNode.class, name = "SimpleTypeNode"),
		@JsonSubTypes.Type(value = SubroutineTypeNode.class, name = "SubroutineTypeNode")
})
public interface IBasicAndWrappedTypeNode extends IASTNode, ITypeNode {

	//classes implementing this interface are also used to communicate type information throughout the compiler.
	//so it is not just for parsing.

	String getTypeName();
}
