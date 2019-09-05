package org.vanautrui.languages.dragoncompiler.parsing.astnodes.nonterminal;

import org.vanautrui.languages.dragoncompiler.lexing.utils.TokenList;
import org.vanautrui.languages.dragoncompiler.lexing.utils.IToken;
import org.vanautrui.languages.dragoncompiler.lexing.tokens.OperatorToken;
import org.vanautrui.languages.dragoncompiler.parsing.IASTNode;

public class OperatorNode implements IASTNode {

    //same as the token

    public String operator;

    public OperatorNode(TokenList tokens)throws Exception{

        TokenList copy = new TokenList(tokens);

        IToken token = copy.get(0);

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
