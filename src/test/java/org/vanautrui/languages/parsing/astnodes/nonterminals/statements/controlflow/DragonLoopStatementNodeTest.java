package org.vanautrui.languages.parsing.astnodes.nonterminals.statements.controlflow;

import org.junit.Test;
import org.vanautrui.languages.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.lexing.tokens.KeywordToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.DragonLoopStatementNode;

public class DragonLoopStatementNodeTest {

    @Test
    public void test1() throws Exception {
        DragonTokenList list = new DragonTokenList();
        list.add(new KeywordToken("loop"));
        list.add(new IntegerNonNegativeConstantToken(5));
        list.add(new SymbolToken("{"));

        list.add(new SymbolToken("}"));

        DragonLoopStatementNode loop = new DragonLoopStatementNode(list);
    }
}
