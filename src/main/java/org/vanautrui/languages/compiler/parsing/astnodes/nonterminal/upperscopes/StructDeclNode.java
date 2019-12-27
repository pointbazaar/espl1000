package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes;

import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.StructMemberDeclNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.IBasicAndWrappedTypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.SimpleTypeNode;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public final class StructDeclNode implements IASTNode, IBasicAndWrappedTypeNode {

	public final SimpleTypeNode structType;
	public final List<StructMemberDeclNode> structMembersList;

	public StructDeclNode(final SimpleTypeNode structType, final List<StructMemberDeclNode> structMemberDeclNodes){
		this.structType=structType;
		this.structMembersList=structMemberDeclNodes;
	}

	@Override
	public String getTypeName() {
		return this.structType.getTypeName();
	}

	@Override
	public String toSourceCode() {
		return "struct " + this.getTypeName() + "{" + this.structMembersList.stream().map(member -> member.toSourceCode()).collect(Collectors.joining(",")) + "}";
	}

	@Override
	public boolean isPrimitive() {
		return false;
	}
}
