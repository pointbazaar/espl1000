package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.StructMemberDeclNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SimpleTypeNode;

import java.util.List;
import java.util.stream.Collectors;

public final class StructDeclNode implements IASTNode {

	public SimpleTypeNode typeNode;
	public List<StructMemberDeclNode> members;

	public StructDeclNode(){}

	public StructDeclNode(final SimpleTypeNode typeNode, final List<StructMemberDeclNode> structMemberDeclNodes){
		this.typeNode = typeNode;
		this.members =structMemberDeclNodes;
	}

	/*
	@Override
	public String getTypeName() {
		return this.type.getTypeName();
	}

	 */

	@Override
	public String toSourceCode() {
		return "struct " + this.typeNode.toSourceCode() + "{" + this.members.stream().map(member -> member.toSourceCode()).collect(Collectors.joining(",")) + "}";
	}

	/*
	@Override
	public boolean isPrimitive() {
		return false;
	}

	 */
}
