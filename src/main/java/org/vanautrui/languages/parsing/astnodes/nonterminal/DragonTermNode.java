package org.vanautrui.languages.parsing.astnodes.nonterminal;

import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.lexing.tokens.IntegerConstantToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonIntegerConstantNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonStringConstantNode;

import java.util.Optional;
import java.util.Set;

public class DragonTermNode implements IDragonASTNode {

    //can be one of these:

    //(keyword constant means true|false|null|this

    /*

    integer constant
    string constant
    keyword constant
    '(' expression ')'
    unaryOp term

     */

    public Optional<DragonIntegerConstantNode> integerConstantNode=Optional.empty();
    public Optional<DragonStringConstantNode> stringConstantNode=Optional.empty();
    public Optional<DragonExpressionNode> expressionNode=Optional.empty();

    //TODO: insert the oher alternatives

    public DragonTermNode (DragonTokenList tokens)throws Exception{

        DragonTokenList copy = new DragonTokenList(tokens);

        try{
            this.integerConstantNode=Optional.of(new DragonIntegerConstantNode(copy));
        }catch (Exception e1){
            try {
                this.stringConstantNode=Optional.of(new DragonStringConstantNode(copy));
            }catch (Exception e2){
                copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("("));
                this.expressionNode=Optional.of(new DragonExpressionNode(copy));
                copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(")"));
            }
        }

        tokens.set(copy);
    }

    @Override
    public void doTypeCheck(Set<DragonAST> asts, Optional<DragonClassNode> currentClass, Optional<DragonMethodNode> currentMethod) throws Exception {
        if(this.integerConstantNode.isPresent()){
            this.integerConstantNode.get().doTypeCheck(asts,currentClass,currentMethod);
        }
        if(this.stringConstantNode.isPresent()){
            this.stringConstantNode.get().doTypeCheck(asts,currentClass,currentMethod);
        }
        this.expressionNode.get().doTypeCheck(asts, currentClass, currentMethod);
    }

    @Override
    public String toSourceCode() {
        if(this.integerConstantNode.isPresent()){
            return this.integerConstantNode.get().toSourceCode();
        }
        if(this.stringConstantNode.isPresent()){
            return this.stringConstantNode.get().toSourceCode();
        }
        return this.expressionNode.get().toSourceCode();
    }
}
