package org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow;

import org.vanautrui.languages.lexing.tokens.KeywordToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.parsing.IASTNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.IStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.StatementNode;

import java.util.ArrayList;
import java.util.List;

public class LoopStatementNode implements IASTNode, IStatementNode {

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

    public ExpressionNode count;

    public List<StatementNode> statements=new ArrayList<>();

    public LoopStatementNode(DragonTokenList tokens)throws Exception{

        DragonTokenList copy = new DragonTokenList(tokens);

        copy.expectAndConsumeOtherWiseThrowException(new KeywordToken("loop"));

        this.count=new ExpressionNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("{"));

        //maybe there be some statements
        boolean success_statements = true;
        while (success_statements) {
            try {
                this.statements.add(new StatementNode(copy));
            } catch (Exception e) {
                success_statements = false;
            }
        }

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("}"));

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return " loop "+this.count.toSourceCode()+" { "+this.statements.stream().map(stmt->stmt.toSourceCode())+" } ";
    }
}
