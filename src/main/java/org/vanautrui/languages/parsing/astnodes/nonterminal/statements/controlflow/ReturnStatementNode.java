package org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow;

import org.simpleframework.xml.Attribute;
import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.lexing.tokens.KeywordToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.IASTNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.IStatementNode;

import java.util.Optional;

public class ReturnStatementNode implements IASTNode, IStatementNode {

    @Attribute
    public String getReturnValueStringForXML(){
        if(returnValue.isPresent()){
            return returnValue.get().toSourceCode();
        }else{
            return "";
        }
    }

    public Optional<ExpressionNode> returnValue=Optional.empty();

    public ReturnStatementNode(TokenList tokens)throws Exception{

        TokenList copy = new TokenList(tokens);

        copy.expectAndConsumeOtherWiseThrowException(new KeywordToken("return"));

        try {
            this.returnValue = Optional.of(new ExpressionNode(copy));
        }catch (Exception e){
            //pass
        }

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(";"));

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        if(this.returnValue.isPresent()) {
            return " return " + this.returnValue.get().toSourceCode() + " ; ";
        }else {
            return " return; ";
        }
    }
}
