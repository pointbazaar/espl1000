package org.vanautrui.languages.parsing.astnodes.nonterminal;

import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.lexing.tokens.utils.Token;
import org.vanautrui.languages.lexing.tokens.OperatorToken;
import org.vanautrui.languages.parsing.IASTNode;

public class OperatorNode implements IASTNode {

    //same as the token

    public String operator;

    public OperatorNode(TokenList tokens)throws Exception{

        TokenList copy = new TokenList(tokens);

        Token token = copy.get(0);

        if(token instanceof OperatorToken){
            this.operator=((OperatorToken)token).operator;
            copy.consume(1);
        }else{
            throw new Exception("could not recognize operator token: got "+token.getContents());
        }

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return " "+operator+" ";
    }
}
