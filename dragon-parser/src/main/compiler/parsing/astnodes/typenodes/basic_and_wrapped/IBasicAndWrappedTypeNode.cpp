
//standard headers
#include <vector>
#include <set>

//project headers

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
}
