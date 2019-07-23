package org.vanautrui.languages.parsing.astnodes;

import org.vanautrui.languages.lexing.tokens.DragonToken;
import org.vanautrui.languages.parsing.IDragonASTNode;

import java.util.ArrayList;
import java.util.List;

public class DragonArgumentNode implements IDragonASTNode {

    public DragonTypeNode type;

    public DragonIdentifierNode name;

    public DragonArgumentNode(List<DragonToken> tokens)throws Exception{

        List<DragonToken> copy = new ArrayList<>(tokens);

        this.type=new DragonTypeNode(copy);

        this.name=new DragonIdentifierNode(copy);

        tokens.clear();
        tokens.addAll(copy);
    }

    @Override
    public String toSourceCode() {
        return null;
    }
}
