package org.vanautrui.languages.parsing.astnodes;

import org.vanautrui.languages.lexing.tokens.DragonToken;
import org.vanautrui.languages.parsing.IDragonASTNode;

import java.util.List;

public class DragonMethodNameNode implements IDragonASTNode {

    public DragonIdentifierNode methodName;

    public DragonMethodNameNode(List<DragonToken> tokens) throws Exception{

        this.methodName =new DragonIdentifierNode(tokens);

    }


    @Override
    public String toSourceCode() {
        return this.methodName.toSourceCode();
    }
}
