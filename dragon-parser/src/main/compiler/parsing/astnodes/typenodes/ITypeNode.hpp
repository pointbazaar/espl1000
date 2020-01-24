
//standard headers
#include <vector>
#include <set>

//project headers

/*
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
*/

class ITypeNode : IASTNode {

};