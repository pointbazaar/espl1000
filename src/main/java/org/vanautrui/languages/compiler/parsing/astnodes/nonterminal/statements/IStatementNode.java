package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements;

import com.fasterxml.jackson.annotation.JsonSubTypes;
import com.fasterxml.jackson.annotation.JsonTypeInfo;
import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;

@JsonTypeInfo(
		use = JsonTypeInfo.Id.NAME,
		include = JsonTypeInfo.As.PROPERTY,
		property = "type")
@JsonSubTypes({
		@JsonSubTypes.Type(value = IfStatementNode.class, name = "IfStatementNode"),
		@JsonSubTypes.Type(value = WhileStatementNode.class, name = "WhileStatementNode"),
		@JsonSubTypes.Type(value = LoopStatementNode.class, name = "LoopStatementNode"),
		@JsonSubTypes.Type(value = ReturnStatementNode.class, name = "ReturnStatementNode"),
		@JsonSubTypes.Type(value = AssignmentStatementNode.class, name = "AssignmentStatementNode")
})
public interface IStatementNode extends IASTNode {

	//so the different statements do not have to have seperate
	//field in the DragonStatementNode AST node that is
	//parsing
}
