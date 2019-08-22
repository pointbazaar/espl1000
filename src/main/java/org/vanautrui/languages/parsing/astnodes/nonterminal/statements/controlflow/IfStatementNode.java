package org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow;

import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.lexing.tokens.KeywordToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.IASTNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.IStatementNode;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class IfStatementNode implements IASTNode, IStatementNode {

    public ExpressionNode condition;

    public List<StatementNode> statements=new ArrayList<>();

    public List<StatementNode> elseStatements = new ArrayList<>();

    public IfStatementNode(DragonTokenList tokens)throws Exception{

        DragonTokenList copy = new DragonTokenList(tokens);

        copy.expectAndConsumeOtherWiseThrowException(new KeywordToken("if"));

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("("));

        this.condition =new ExpressionNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(")"));

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

        //maybe there is an else

        if(copy.startsWith(new KeywordToken("else"))){

            copy.expectAndConsumeOtherWiseThrowException(new KeywordToken("else"));

            copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("{"));

            //maybe there be some statements
            boolean success_statements2 = true;
            while (success_statements2) {
                try {
                    this.elseStatements.add(new StatementNode(copy));
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
