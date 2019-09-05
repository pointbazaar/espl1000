package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow;

import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.lexing.tokens.KeywordToken;
import org.vanautrui.languages.compiler.lexing.tokens.SymbolToken;
import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.StatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.IStatementNode;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class WhileStatementNode implements IASTNode, IStatementNode {

    public ExpressionNode condition;

    public List<StatementNode> statements=new ArrayList<>();

    public WhileStatementNode(TokenList tokens)throws Exception{

        TokenList copy = new TokenList(tokens);

        copy.expectAndConsumeOtherWiseThrowException(new KeywordToken("while"));

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

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return " while ( "+this.condition.toSourceCode()+" ) { "+this.statements.stream().map(stmt->stmt.toSourceCode()).collect(Collectors.joining(" "))+" } ";
    }
}
