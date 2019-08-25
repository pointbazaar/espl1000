package org.vanautrui.languages.parsing.astnodes.terminal;

import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.lexing.tokens.utils.IToken;
import org.vanautrui.languages.parsing.IASTNode;
import org.vanautrui.languages.parsing.astnodes.ITermNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.ExpressionNode;

import java.util.Optional;

public class VariableNode implements IASTNode, ITermNode {

    //this identifies a variable

    public String name;

    public Optional<ExpressionNode> indexOptional=Optional.empty();

    public VariableNode(TokenList tokens) throws Exception {

        TokenList copy=new TokenList(tokens);

        IToken token = copy.get(0);

        if (token instanceof IdentifierToken) {
            this.name = ((IdentifierToken) token).getContents();
            copy.consume(1);

            try{
                TokenList copy2 = new TokenList(copy);

                copy2.expectAndConsumeOtherWiseThrowException(new SymbolToken("["));
                this.indexOptional= Optional.of(new ExpressionNode(copy2));
                copy2.expectAndConsumeOtherWiseThrowException(new SymbolToken("]"));

                copy.set(copy2);
            }catch (Exception e){
                //pass, this assignment has no index to it
            }

        } else {
            throw new Exception("could not read variable name");
        }
        tokens.set(copy);

    }

    @Override
    public String toSourceCode() {
        if(this.indexOptional.isPresent()){
            return this.name+"["+this.indexOptional.get().toSourceCode()+"]";
        }
        return this.name;
    }

}
