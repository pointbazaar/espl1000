package org.vanautrui.languages.parsing.astnodes.terminal;

import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.lexing.tokens.utils.Token;
import org.vanautrui.languages.parsing.IASTNode;
import org.vanautrui.languages.parsing.astnodes.ITermNode;

public class VariableNode implements IASTNode, ITermNode {

    //this identifies a variable

    public String name;

    public VariableNode(TokenList tokens) throws Exception {

        Token token = tokens.get(0);

        if (token instanceof IdentifierToken) {
            this.name = ((IdentifierToken) token).getContents();
            tokens.consume(1);

        } else {
            throw new Exception("could not read variable name");
        }

    }

    @Override
    public String toSourceCode() {
        return this.name;
    }

}
