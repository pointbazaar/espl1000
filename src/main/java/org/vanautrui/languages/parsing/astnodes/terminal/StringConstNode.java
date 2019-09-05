package org.vanautrui.languages.parsing.astnodes.terminal;

import org.vanautrui.languages.lexing.utils.TokenList;
import org.vanautrui.languages.lexing.tokens.StringConstantToken;
import org.vanautrui.languages.lexing.utils.IToken;
import org.vanautrui.languages.parsing.IASTNode;
import org.vanautrui.languages.parsing.astnodes.ITermNode;

public class StringConstNode implements IASTNode, ITermNode {

    public String str;

    public StringConstNode(TokenList tokens) throws Exception {

        IToken token = tokens.get(0);

        if (token instanceof StringConstantToken) {
            this.str = ((StringConstantToken) token).getContents();
            tokens.consume(1);
        } else {
            throw new Exception("could not read stringConstant node");
        }

    }

    @Override
    public String toSourceCode() {
        return this.str;
    }
}
