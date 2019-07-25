package org.vanautrui.languages.parsing.astnodes.terminal;

import org.vanautrui.languages.lexing.tokens.DragonToken;
import org.vanautrui.languages.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.lexing.tokens.StringConstantToken;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;

public class DragonStringConstantNode implements IDragonASTNode {

    public String str;

    public DragonStringConstantNode(DragonTokenList tokens)throws Exception{

        DragonToken token = tokens.get(0);

        if(token instanceof StringConstantToken){
            this.str =((StringConstantToken)token).getContents();
            tokens.consume(1);
        }else{
            throw new Exception("could not read stringConstant node");
        }

    }

    @Override
    public String toSourceCode() {
        return this.str;
    }
}
