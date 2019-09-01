package org.vanautrui.languages.parsing.astnodes.terminal;

import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.lexing.tokens.*;
import org.vanautrui.languages.parsing.IASTNode;
import org.vanautrui.languages.parsing.astnodes.ITermNode;

public class BoolConstNode implements IASTNode, ITermNode {

    //this can also have a negative value if it is preceded by a '-' operator token

    public boolean value;

    public BoolConstNode(TokenList tokens) throws Exception {

        TokenList copy = new TokenList(tokens);

        if(copy.get(0) instanceof BoolConstantToken){
            BoolConstantToken tk = (BoolConstantToken)copy.get(0);
			this.value=tk.value;
			copy.consume(1);
		} else {
            throw new Exception("could not read Bool Constant node");
        }

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return this.value+"";
    }

}
