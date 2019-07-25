package org.vanautrui.languages.parsing.astnodes.terminal;

import org.vanautrui.languages.lexing.tokens.DragonToken;
import org.vanautrui.languages.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;

public class DragonIdentifierNode implements IDragonASTNode {

    public String name;

    public DragonIdentifierNode(DragonTokenList tokens)throws Exception{

        //List<DragonToken> copy = new ArrayList<>(tokens);

        //Pattern p = Pattern.compile(IdentifierToken.regex_alphanumeric_identifier);

        DragonToken token = tokens.get(0);

        if(token instanceof IdentifierToken){
            this.name=((IdentifierToken)token).getContents();
            tokens.consume(1);

        }else{
            throw new Exception("could not read identifier");
        }

    }

    @Override
    public String toSourceCode() {
        return this.name;
    }
}
