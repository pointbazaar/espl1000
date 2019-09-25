package org.vanautrui.languages.compiler.parsing.astnodes.terminal;

import org.vanautrui.languages.compiler.lexing.tokens.*;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.ITermNode;

public class FloatConstNode implements IASTNode, ITermNode {

    //this can also have a negative value if it is preceded by a '-' operator token
    public final float value;

    public FloatConstNode(TokenList tokens) throws Exception {

        TokenList copy = new TokenList(tokens);

        if(copy.get(0) instanceof OperatorToken){
            OperatorToken tk = (OperatorToken)copy.get(0);
            if(tk.operator.equals("-") && (copy.get(1) instanceof FloatNonNegativeConstantToken) ){
                this.value=-(((FloatNonNegativeConstantToken) copy.get(1)).value);
                copy.consume(2);
            }else{
                throw new Exception("cannot parse float constant node with such operator:"+tk.operator);
            }
        }else if (copy.get(0) instanceof FloatNonNegativeConstantToken) {
            this.value = ((FloatNonNegativeConstantToken) copy.get(0)).value;
            copy.consume(1);
        } else {
            throw new Exception("could not read float constant node");
        }

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return this.value+"";
    }

}
