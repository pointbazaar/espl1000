package org.vanautrui.languages.parsing.astnodes.nonterminals.statements.controlflow;

import org.junit.Test;
import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.lexing.tokens.KeywordToken;
import org.vanautrui.languages.lexing.tokens.OperatorToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;

public class WhileStatementNodeTest {

    @Test
    public void test1() throws Exception {
        TokenList list = new TokenList();
        list.add(new KeywordToken("while"));

        list.add(new SymbolToken("("));

        list.add(new IntegerNonNegativeConstantToken(5));
        list.add(new OperatorToken("<"));
        list.add(new IntegerNonNegativeConstantToken(3));

        list.add(new SymbolToken(")"));

        list.add(new SymbolToken("{"));

        list.add(new SymbolToken("}"));

        WhileStatementNode whileStatement = new WhileStatementNode(list);
    }
}
