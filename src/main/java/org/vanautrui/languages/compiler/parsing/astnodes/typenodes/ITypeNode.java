package org.vanautrui.languages.compiler.parsing.astnodes.typenodes;

import com.fasterxml.jackson.annotation.JsonSubTypes;
import com.fasterxml.jackson.annotation.JsonTypeInfo;
import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.BasicTypeWrappedNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SimpleTypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SubroutineTypeNode;

@JsonTypeInfo(
		use = JsonTypeInfo.Id.NAME,
		include = JsonTypeInfo.As.PROPERTY,
		property = "type")
@JsonSubTypes({
		@JsonSubTypes.Type(value = ArrayTypeNode.class, name = "ArrayTypeNode"),
		@JsonSubTypes.Type(value = TypeParameterNode.class, name = "TypeParameterNode"),
		@JsonSubTypes.Type(value = BasicTypeWrappedNode.class, name = "BasicTypeWrappedNode"),

		@JsonSubTypes.Type(value = TypeParameterNode.class, name = "TypeParameterNode"),
		@JsonSubTypes.Type(value = SimpleTypeNode.class, name = "SimpleTypeNode"),
		@JsonSubTypes.Type(value = SubroutineTypeNode.class, name = "SubroutineTypeNode")
})
public interface ITypeNode extends IASTNode {

	//such we can see if a type is primitive,
	//being passed by value instead of by reference,
	boolean isPrimitive();
}
