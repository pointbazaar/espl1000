package org.vanautrui.languages.parsing.astnodes.nonterminal;

import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;

public class DragonStatementNode implements IDragonASTNode {

    //TODO: add more statement types

    public DragonMethodCallNode methodCallNode;

    public DragonStatementNode(DragonTokenList tokens) throws Exception {
        //System.out.println("try parse DragonStatementNode");

        DragonTokenList copy = tokens.copy();

        this.methodCallNode = new DragonMethodCallNode(copy);

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return this.methodCallNode.toSourceCode();
    }
}
