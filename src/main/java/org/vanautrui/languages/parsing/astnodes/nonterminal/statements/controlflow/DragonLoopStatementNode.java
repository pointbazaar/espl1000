package org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow;

import org.vanautrui.languages.lexing.tokens.KeywordToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonExpressionNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.IDragonStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonStatementNode;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.Set;

public class DragonLoopStatementNode implements IDragonASTNode, IDragonStatementNode {

    /*
    loop 3 {
        println("helol");
    }
    x=4
    loop x {
        println("x");
    }
     */

    //this statement serves to repeat some statements
    //a number of times which is known ahead of the loop statement

    //at first we focus just on the case of a fixed
    //repetition count known ahead of compile time

    public DragonExpressionNode count;

    public List<DragonStatementNode> statements=new ArrayList<>();

    public DragonLoopStatementNode(DragonTokenList tokens)throws Exception{

        DragonTokenList copy = new DragonTokenList(tokens);

        copy.expectAndConsumeOtherWiseThrowException(new KeywordToken("loop"));

        this.count=new DragonExpressionNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("{"));

        //maybe there be some statements
        boolean success_statements = true;
        while (success_statements) {
            try {
                this.statements.add(new DragonStatementNode(copy));
            } catch (Exception e) {
                success_statements = false;
            }
        }

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("}"));

        tokens.set(copy);
    }

    @Override
    public void doTypeCheck(Set<DragonAST> asts, Optional<DragonClassNode> currentClass, Optional<DragonMethodNode> currentMethod) throws Exception {
        //the condition expression should be of type boolean
        if(!this.count.getType().equals("Int")){
            throw new Exception(" condition should be of type Int . this is a loop statement after all.");
        }
        for(DragonStatementNode stmt : this.statements){
            stmt.doTypeCheck(asts,currentClass,currentMethod);
        }
    }

    @Override
    public String toSourceCode() {
        return " loop "+this.count.toSourceCode()+" { "+this.statements.stream().map(stmt->stmt.toSourceCode())+" } ";
    }
}
