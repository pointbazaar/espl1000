package org.vanautrui.languages.parsing.astnodes.nonterminal.statements;

import org.vanautrui.languages.lexing.tokens.KeywordToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonMethodNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonStatementNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonIntegerConstantNode;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.Set;

public class DragonLoopStatementNode implements IDragonASTNode,IDragonStatementNode {

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

    public DragonIntegerConstantNode count;

    public List<DragonStatementNode> statements=new ArrayList<>();

    public DragonLoopStatementNode(DragonTokenList tokens)throws Exception{

        DragonTokenList copy = new DragonTokenList(tokens);

        copy.expectAndConsumeOtherWiseThrowException(new KeywordToken("loop"));

        this.count=new DragonIntegerConstantNode(copy);

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
        for(DragonStatementNode stmt : this.statements){
            stmt.doTypeCheck(asts,currentClass,currentMethod);
        }
    }

    @Override
    public String toSourceCode() {
        return " loop "+this.count.toSourceCode()+" { "+this.statements.stream().map(stmt->stmt.toSourceCode())+" } ";
    }
}
