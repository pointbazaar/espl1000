package org.vanautrui.languages.parsing.astnodes.nonterminals.statements.controlflow;

import org.junit.Test;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.lexing.tokens.IntegerConstantToken;
import org.vanautrui.languages.lexing.tokens.KeywordToken;
import org.vanautrui.languages.lexing.tokens.OperatorToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.DragonIfStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.DragonWhileStatementNode;

public class DragonIfStatementNodeTest {

    @Test
    public void test1() throws Exception {
        DragonTokenList list = new DragonTokenList();
        list.add(new KeywordToken("if"));

        list.add(new SymbolToken("("));

        list.add(new IntegerConstantToken(5));
        list.add(new OperatorToken("<"));
        list.add(new IntegerConstantToken(3));

        list.add(new SymbolToken(")"));

        list.add(new SymbolToken("{"));

        list.add(new SymbolToken("}"));

        DragonIfStatementNode ifStatement = new DragonIfStatementNode(list);
    }
}
