package org.vanautrui.languages.dragoncompiler.parsing.astnodes.nonterminal;

import org.vanautrui.languages.dragoncompiler.lexing.utils.TokenList;
import org.vanautrui.languages.dragoncompiler.lexing.tokens.SymbolToken;
import org.vanautrui.languages.dragoncompiler.parsing.IASTNode;
import org.vanautrui.languages.dragoncompiler.parsing.astnodes.ITermNode;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class ArrayConstantNode implements IASTNode, ITermNode {

    //same as the token

    public List<ExpressionNode> elements=new ArrayList<>();

    public ArrayConstantNode(TokenList tokens)throws Exception{

        TokenList copy = new TokenList(tokens);

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("["));

        try{
            this.elements.add(new ExpressionNode(copy));
            boolean success=true;
            while (success){
                try {
                    TokenList copy2 = new TokenList(copy);

                    copy2.expectAndConsumeOtherWiseThrowException(new SymbolToken(","));
                    this.elements.add(new ExpressionNode(copy2));

                    copy.set(copy2);
                }catch (Exception ee){
                    success=false;
                }
            }
        }catch (Exception e){
            //pass
        }

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("]"));

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return " ["+this.elements.stream().map(ExpressionNode::toSourceCode).collect(Collectors.joining(","))+"] ";
    }
}
