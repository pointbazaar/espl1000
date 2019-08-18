package org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow;

import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.lexing.tokens.KeywordToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonExpressionNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.IDragonStatementNode;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

public class DragonIfStatementNode implements IDragonASTNode, IDragonStatementNode {

    public DragonExpressionNode condition;

    public List<DragonStatementNode> statements=new ArrayList<>();

    public List<DragonStatementNode> elseStatements = new ArrayList<>();

    public DragonIfStatementNode(DragonTokenList tokens)throws Exception{

        DragonTokenList copy = new DragonTokenList(tokens);

        copy.expectAndConsumeOtherWiseThrowException(new KeywordToken("if"));

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("("));

        this.condition =new DragonExpressionNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(")"));

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

        //maybe there is an else

        if(copy.startsWith(new KeywordToken("else"))){

            copy.expectAndConsumeOtherWiseThrowException(new KeywordToken("else"));

            copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("{"));

            //maybe there be some statements
            boolean success_statements2 = true;
            while (success_statements2) {
                try {
                    this.elseStatements.add(new DragonStatementNode(copy));
                } catch (Exception e) {
                    success_statements2 = false;
                }
            }

            copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("}"));
        }

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return " if ( "+this.condition.toSourceCode()+" ) { "+this.statements.stream().map(stmt->stmt.toSourceCode()).collect(Collectors.joining(" "))+" } ";
    }
}
