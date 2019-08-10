package org.vanautrui.languages.parsing.astnodes;

import org.junit.Test;
import org.vanautrui.languages.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonMethodCallNode;

public class DragonMethodCallNodeTest {

    @Test
    public void test1() throws Exception {
        DragonTokenList list = new DragonTokenList();
        list.add(new IdentifierToken("main"));
        list.add(new SymbolToken("("));
        list.add(new SymbolToken(")"));
        list.add(new SymbolToken(";"));

        DragonMethodCallNode call = new DragonMethodCallNode(list);
    }
}
