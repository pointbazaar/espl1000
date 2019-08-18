package org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow;

import org.simpleframework.xml.Attribute;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.lexing.tokens.KeywordToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonExpressionNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.IDragonStatementNode;

import java.util.Optional;
import java.util.stream.Collectors;

public class DragonReturnStatementNode implements IDragonASTNode, IDragonStatementNode {

    @Attribute
    public String getReturnValueStringForXML(){
        if(returnValue.isPresent()){
            return returnValue.get().toSourceCode();
        }else{
            return "";
        }
    }

    public Optional<DragonExpressionNode> returnValue=Optional.empty();

    public DragonReturnStatementNode(DragonTokenList tokens)throws Exception{

        DragonTokenList copy = new DragonTokenList(tokens);

        copy.expectAndConsumeOtherWiseThrowException(new KeywordToken("return"));

        try {
            this.returnValue = Optional.of(new DragonExpressionNode(copy));
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
