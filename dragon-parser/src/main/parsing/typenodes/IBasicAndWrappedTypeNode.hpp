#ifndef IBASICANDWRAPPEDTYPENODE 
#define IBASICANDWRAPPEDTYPENODE

//standard headers
#include <vector>
#include <set>

//project headers
#include "ITypeNode.hpp"

/*
@JsonTypeInfo(
		use = JsonTypeInfo.Id.NAME,
		include = JsonTypeInfo.As.PROPERTY,
		property = "type")
@JsonSubTypes({
		@JsonSubTypes.Type(value = SimpleTypeNode.class, name = "SimpleTypeNode"),
		@JsonSubTypes.Type(value = SubroutineTypeNode.class, name = "SubroutineTypeNode")
})
*/

using namespace std;

class IBasicAndWrappedTypeNode : public ITypeNode {

	//classes implementing this interface are also used to communicate type information throughout the compiler.
	//so it is not just for parsing.
};

#endif