package org.vanautrui.languages.parsing.astnodes.terminal;

import org.vanautrui.languages.lexing.tokens.utils.Token;
import org.vanautrui.languages.lexing.tokens.TypeIdentifierToken;
import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.parsing.IASTNode;

public class TypeIdentifierNode implements IASTNode {

    public String typeName;

    public TypeIdentifierNode(TokenList tokens) throws Exception {

        Token token = tokens.get(0);

        if (token instanceof TypeIdentifierToken) {
            this.typeName = ((TypeIdentifierToken) token).getContents();
            tokens.consume(1);

        } else {
            throw new Exception("could not read type identifier");
        }
    }


    @Override
    public String toSourceCode() {
        return this.typeName;
    }

}
