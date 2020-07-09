package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements;

import org.vanautrui.languages.compiler.parsing.IASTNode;

public interface IStatementNode extends IASTNode {
	boolean containsSubroutineCalls();

	//so the different statements do not have to have seperate
	//field in the DragonStatementNode AST node that is
	//parsing
}
