package org.vanautrui.languages.parsing.astnodes.terminal;

import org.vanautrui.languages.lexing.utils.TokenList;
import org.vanautrui.languages.lexing.tokens.OperatorToken;
import org.vanautrui.languages.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.parsing.IASTNode;
import org.vanautrui.languages.parsing.astnodes.ITermNode;

public class IntConstNode implements IASTNode, ITermNode {

    //this can also have a negative value if it is preceded by a '-' operator token

    public int value;

    public IntConstNode(TokenList tokens) throws Exception {

        TokenList copy = new TokenList(tokens);

        if(copy.get(0) instanceof OperatorToken){
            OperatorToken tk = (OperatorToken)copy.get(0);
            if(tk.operator.equals("-") && (copy.get(1) instanceof IntegerNonNegativeConstantToken) ){
                this.value=-(((IntegerNonNegativeConstantToken) copy.get(1)).value);
                copy.consume(2);
            }else{
                throw new Exception("cannot parse integer constant node with such operator:"+tk.operator);
            }
        }else if (copy.get(0) instanceof IntegerNonNegativeConstantToken) {
            this.value = ((IntegerNonNegativeConstantToken) copy.get(0)).value;
            copy.consume(1);
        } else {
            throw new Exception("could not read stringConstant node");
        }

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return this.value+"";
    }

}
