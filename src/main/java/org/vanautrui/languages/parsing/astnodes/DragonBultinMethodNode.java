package org.vanautrui.languages.parsing.astnodes;

import org.vanautrui.languages.lexing.tokens.DragonToken;
import org.vanautrui.languages.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.parsing.IDragonASTNode;

import java.util.Arrays;
import java.util.List;

public class DragonBultinMethodNode implements IDragonASTNode {

    public final static List<String> builtin_methods = Arrays.asList(
            "print","println"
    );

    public DragonIdentifierNode methodname;

    public DragonBultinMethodNode(List<DragonToken> tokens)throws Exception{

        DragonToken token = tokens.get(0);

        if(token instanceof IdentifierToken){
            IdentifierToken identifierToken = (IdentifierToken)token;
            if(builtin_methods.contains(identifierToken.getContents())){
                this.methodname=new DragonIdentifierNode(tokens);
            }
        }
    }

    //TODO
    @Override
    public String toSourceCode() {
        return this.methodname.toSourceCode();
    }
}
