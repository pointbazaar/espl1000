package org.vanautrui.languages.parsing.astnodes.terminal;

import org.simpleframework.xml.Attribute;
import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.lexing.tokens.utils.Token;
import org.vanautrui.languages.lexing.tokens.StringConstantToken;
import org.vanautrui.languages.parsing.IASTNode;
import org.vanautrui.languages.parsing.astnodes.ITermNode;

public class StringConstantNode implements IASTNode, ITermNode {

    @Attribute
    public String str;

    public StringConstantNode(TokenList tokens) throws Exception {

        Token token = tokens.get(0);

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
