package org.vanautrui.languages.compiler.parsing.astnodes.terminal;

import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.IASTNode;

public class MethodNameNode implements IASTNode {

    public final String methodName;

    public MethodNameNode(TokenList tokens) throws Exception {
        this.methodName = new IdentifierNode(tokens).name;
    }


    @Override
    public String toSourceCode() {
        return this.methodName;
    }

}
