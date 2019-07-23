package org.vanautrui.languages.parsing.astnodes;

import org.vanautrui.languages.lexing.tokens.DragonToken;
import org.vanautrui.languages.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;

import java.util.ArrayList;
import java.util.List;

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
