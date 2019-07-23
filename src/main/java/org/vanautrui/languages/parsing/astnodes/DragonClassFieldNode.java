package org.vanautrui.languages.parsing.astnodes;

import org.vanautrui.languages.lexing.tokens.AccessModifierToken;
import org.vanautrui.languages.lexing.tokens.DragonToken;
import org.vanautrui.languages.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

public class DragonClassFieldNode implements IDragonASTNode {

    public DragonAccessModifierNode access;

    public DragonTypeNode type;

    public DragonIdentifierNode name;

    public DragonClassFieldNode(DragonTokenList tokens)throws Exception{
        System.out.println("try parse DragonClassFieldNode");

        DragonTokenList copy = tokens.copy();

        try{
            this.access=new DragonAccessModifierNode(copy);
        }catch (Exception e){
            //pass
        }

        this.type=new DragonTypeNode(copy);

        this.name=new DragonIdentifierNode(copy);

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return null;
    }
}
