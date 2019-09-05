package org.vanautrui.languages.dragoncompiler.parsing.astnodes.terminal;

import org.vanautrui.languages.dragoncompiler.lexing.utils.IToken;
import org.vanautrui.languages.dragoncompiler.lexing.tokens.TypeIdentifierToken;
import org.vanautrui.languages.dragoncompiler.lexing.utils.TokenList;
import org.vanautrui.languages.dragoncompiler.parsing.IASTNode;

public class TypeIdentifierNode implements IASTNode {

    public String typeName;

    public TypeIdentifierNode(TokenList tokens) throws Exception {

        IToken token = tokens.get(0);

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
