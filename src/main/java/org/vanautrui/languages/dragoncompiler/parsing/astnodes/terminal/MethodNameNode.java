package org.vanautrui.languages.dragoncompiler.parsing.astnodes.terminal;

import org.vanautrui.languages.dragoncompiler.lexing.utils.TokenList;
import org.vanautrui.languages.dragoncompiler.parsing.IASTNode;

public class MethodNameNode implements IASTNode {

    public IdentifierNode methodName;

    public MethodNameNode(TokenList tokens) throws Exception {

        this.methodName = new IdentifierNode(tokens);

    }


    @Override
    public String toSourceCode() {
        return this.methodName.toSourceCode();
    }

}
