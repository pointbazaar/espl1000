package org.vanautrui.languages.compiler.parsing.astnodes;

import org.vanautrui.languages.ISourceCode;

public interface ITermNode extends IExpressionComputable, ISourceCode {

	//pseudo interface,
	//only (AST Nodes classes) which are on the right side of a 'term' production should implement it
}
