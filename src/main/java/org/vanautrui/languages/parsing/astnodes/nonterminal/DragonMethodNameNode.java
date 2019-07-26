package org.vanautrui.languages.parsing.astnodes.nonterminal;

import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonIdentifierNode;

public class DragonMethodNameNode implements IDragonASTNode {

    public DragonIdentifierNode methodName;

    public DragonMethodNameNode(DragonTokenList tokens) throws Exception {

        this.methodName = new DragonIdentifierNode(tokens);

    }


    @Override
    public String toSourceCode() {
        return this.methodName.toSourceCode();
    }
}
