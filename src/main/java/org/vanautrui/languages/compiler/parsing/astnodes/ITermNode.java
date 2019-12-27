package org.vanautrui.languages.compiler.parsing.astnodes;

import com.fasterxml.jackson.annotation.JsonSubTypes;
import com.fasterxml.jackson.annotation.JsonTypeInfo;
import org.vanautrui.languages.ISourceCode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ArrayConstantNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.VariableNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.BoolConstNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.CharConstNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.FloatConstNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.IntConstNode;

@JsonTypeInfo(
		use = JsonTypeInfo.Id.NAME,
		include = JsonTypeInfo.As.PROPERTY,
		property = "type")
@JsonSubTypes({
		@JsonSubTypes.Type(value = ArrayConstantNode.class, name = "ArrayConstantNode"),
		@JsonSubTypes.Type(value = ExpressionNode.class, name = "ExpressionNode"),
		@JsonSubTypes.Type(value = VariableNode.class, name = "VariableNode"),
		@JsonSubTypes.Type(value = MethodCallNode.class, name = "MethodCallNode"),
		@JsonSubTypes.Type(value = BoolConstNode.class, name = "BoolConstNode"),
		@JsonSubTypes.Type(value = CharConstNode.class, name = "CharConstNode"),
		@JsonSubTypes.Type(value = FloatConstNode.class, name = "FloatConstNode"),
		@JsonSubTypes.Type(value = IntConstNode.class, name = "IntConstNode")
})
public interface ITermNode extends  ISourceCode {

	//pseudo interface,
	//only (AST Nodes classes) which are on the right side of a 'term' production should implement it
}
