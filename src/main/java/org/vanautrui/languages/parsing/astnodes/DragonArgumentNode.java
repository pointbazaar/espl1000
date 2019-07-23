package org.vanautrui.languages.parsing.astnodes;

import org.vanautrui.languages.lexing.tokens.DragonToken;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;

import java.util.ArrayList;
import java.util.List;

public class DragonArgumentNode implements IDragonASTNode {

    public DragonTypeNode type;

    public DragonIdentifierNode name;

    public DragonArgumentNode(DragonTokenList tokens)throws Exception{

        DragonTokenList copy=tokens.copy();

        this.type=new DragonTypeNode(copy);

        this.name=new DragonIdentifierNode(copy);

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return null;
    }
}
