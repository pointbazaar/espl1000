package org.vanautrui.languages.parsing.astnodes.terminal;

import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;

public class DragonTypeNode implements IDragonASTNode {

    public DragonIdentifierNode typeName;

    public DragonTypeNode(DragonTokenList tokens) throws Exception{

        this.typeName=new DragonIdentifierNode(tokens);

    }


    @Override
    public String toSourceCode() {
        return this.typeName.toSourceCode();
    }
}
