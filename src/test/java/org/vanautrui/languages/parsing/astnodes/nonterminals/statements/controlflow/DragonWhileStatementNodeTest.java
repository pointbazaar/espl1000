package org.vanautrui.languages.parsing.astnodes.nonterminals.statements.controlflow;

import org.junit.Test;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.lexing.tokens.KeywordToken;
import org.vanautrui.languages.lexing.tokens.OperatorToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.DragonWhileStatementNode;

public class DragonWhileStatementNodeTest {

    @Test
    public void test1() throws Exception {
        DragonTokenList list = new DragonTokenList();
        list.add(new KeywordToken("while"));

        list.add(new SymbolToken("("));

        list.add(new IntegerNonNegativeConstantToken(5));
        list.add(new OperatorToken("<"));
        list.add(new IntegerNonNegativeConstantToken(3));

        list.add(new SymbolToken(")"));

        list.add(new SymbolToken("{"));

        list.add(new SymbolToken("}"));

        DragonWhileStatementNode whileStatement = new DragonWhileStatementNode(list);
    }
}
