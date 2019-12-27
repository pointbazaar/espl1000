package org.vanautrui.languages.compiler.parsing.astnodes;

public interface IExpressionComputable {

	//OLD:
	//every computable expression has a type
	//sometimes this needs to be type-checked at compile time
	//this is what this interface is for

	//NEW:
	//this interfaces is deprecated
	//type resolution is not local to the method or even the AST
	//i do not think it belongs in the AST classes
	//therefore the logic has been refactored to elsewhere

	//public String getType(DragonMethodNode methodNode) throws Exception;
}
