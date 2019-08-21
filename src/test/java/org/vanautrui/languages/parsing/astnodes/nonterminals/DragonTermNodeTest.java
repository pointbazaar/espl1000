package org.vanautrui.languages.parsing.astnodes.nonterminals;

import org.junit.Test;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.lexing.tokens.IntegerNonNegativeConstantToken;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonTermNode;

public class DragonTermNodeTest {

    @Test
    public void test_simple_term() throws Exception {

        DragonTokenList list = new DragonTokenList();
        list.add(new IntegerNonNegativeConstantToken(4));
        DragonTermNode expr = new DragonTermNode(list);
    }

    @Test
    public void test_variable_term() throws Exception {

        DragonTokenList list = new DragonTokenList();
        list.add(new IdentifierToken("x"));
        DragonTermNode expr = new DragonTermNode(list);
    }


}
