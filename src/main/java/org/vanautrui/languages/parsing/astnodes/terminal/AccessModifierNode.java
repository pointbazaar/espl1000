package org.vanautrui.languages.parsing.astnodes.terminal;

import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.lexing.tokens.AccessModifierToken;
import org.vanautrui.languages.lexing.tokens.utils.IToken;
import org.vanautrui.languages.parsing.IASTNode;

public class AccessModifierNode implements IASTNode {

    public boolean is_public;

    public AccessModifierNode(TokenList tokens) throws Exception {

        IToken token1 = tokens.get(0);

        if (token1 instanceof AccessModifierToken) {
            this.is_public = ((AccessModifierToken) token1).is_public;
            tokens.consume(1);
        } else {
            //otherwise, it is just public. no access modifier is also an access modifier
            this.is_public = true;
        }
    }

    @Override
    public String toSourceCode() {
        return (is_public) ? "public" : "private";
    }

}
