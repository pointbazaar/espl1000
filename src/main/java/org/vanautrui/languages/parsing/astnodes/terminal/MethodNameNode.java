package org.vanautrui.languages.parsing.astnodes.terminal;

import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.parsing.IASTNode;

public class MethodNameNode implements IASTNode {

    public IdentifierNode methodName;

    public MethodNameNode(DragonTokenList tokens) throws Exception {

        this.methodName = new IdentifierNode(tokens);

    }


    @Override
    public String toSourceCode() {
        return this.methodName.toSourceCode();
    }

}
