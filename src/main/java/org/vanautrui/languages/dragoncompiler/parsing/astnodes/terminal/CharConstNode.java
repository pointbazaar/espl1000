package org.vanautrui.languages.dragoncompiler.parsing.astnodes.terminal;

import org.vanautrui.languages.dragoncompiler.lexing.utils.TokenList;
import org.vanautrui.languages.dragoncompiler.lexing.tokens.CharConstantToken;
import org.vanautrui.languages.dragoncompiler.lexing.utils.IToken;
import org.vanautrui.languages.dragoncompiler.parsing.IASTNode;
import org.vanautrui.languages.dragoncompiler.parsing.astnodes.ITermNode;

public class CharConstNode implements IASTNode, ITermNode {

    public char content;

    public CharConstNode(TokenList tokens) throws Exception {

        IToken token = tokens.get(0);

        if (token instanceof CharConstantToken) {
            this.content = ((CharConstantToken) token).getContents().charAt(0);
            tokens.consume(1);
        } else {
            throw new Exception("could not read charConstant node");
        }

    }

    @Override
    public String toSourceCode() {
        return this.content+"";
    }
}
