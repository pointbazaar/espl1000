package org.vanautrui.languages.compiler.parsing.astnodes.terminal;

import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.lexing.utils.IToken;
import org.vanautrui.languages.compiler.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.compiler.parsing.IASTNode;

public class IdentifierNode implements IASTNode {

    //this identifies a variable

    public String name;

    public IdentifierNode(TokenList tokens) throws Exception {

        //List<DragonToken> copy = new ArrayList<>(tokens);

        //Pattern p = Pattern.compile(IdentifierToken.regex_alphanumeric_identifier);

        IToken token = tokens.get(0);

        if (token instanceof IdentifierToken) {
            this.name = ((IdentifierToken) token).getContents();
            tokens.consume(1);

        } else {
            throw new Exception("could not read identifier");
        }

    }

    @Override
    public String toSourceCode() {
        return this.name;
    }

}
