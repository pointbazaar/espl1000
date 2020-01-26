#ifndef ISTATEMENTNODE
#define ISTATEMENTNODE

//standard headers
#include <vector>

/*
@JsonTypeInfo(
		use = JsonTypeInfo.Id.NAME,
		include = JsonTypeInfo.As.PROPERTY,
		property = "type")
@JsonSubTypes({
		@JsonSubTypes.Type(value = IfStatementNode.class, name = "IfStatementNode"),
		@JsonSubTypes.Type(value = WhileStatementNode.class, name = "WhileStatementNode"),
		@JsonSubTypes.Type(value = LoopStatementNode.class, name = "LoopStatementNode"),
		@JsonSubTypes.Type(value = ReturnStatementNode.class, name = "ReturnStatementNode"),
		@JsonSubTypes.Type(value = AssignmentStatementNode.class, name = "AssignmentStatementNode"),
		@JsonSubTypes.Type(value = MethodCallNode.class, name = "MethodCallNode")
})
*/


class IStatementNode {

	//so the different statements do not have to have seperate
	//field in the DragonStatementNode AST node that is
	//parsing
};


#endif